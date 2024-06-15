#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"
#include "Simulator.h"
#include "SymbolTable.h"
#include "Observer.h"
#include "Vessel.h"
#include "GraphVisualizer.h"
#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <numeric>
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace sim;

Vessel seihr(uint32_t N, std::shared_ptr<SystemState> state) {
    auto v = Vessel{"COVID19 SEIHR: " + std::to_string(N), state};
    const auto eps = 0.0009; // initial fraction of infectious
    const auto I0 = size_t(std::round(eps * N)); // initial infectious
    const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
    const auto S0 = N - I0 - E0; // initial susceptible
    const auto R0 = 2.4; // initial basic reproductive number
    const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
    const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
    const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
    const auto P_H = 0.9e-3; // probability of hospitalization
    const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
    const auto tau = 1.0 / 10.12; // removal rate in hospital (H -> R) ~10.12 days
    const auto S = v.add("S", S0); // susceptible
    const auto E = v.add("E", E0); // exposed
    const auto I = v.add("I", I0); // infectious
    const auto H = v.add("H", 0); // hospitalized
    const auto R = v.add("R", 0); // removed/immune (recovered + dead)
    v.add(S >> (beta / N) >>= E); // susceptible becomes exposed
    v.add((S + I) >> (beta / N) >>= CombinedSpecies({E, I})); // infectious infects susceptible
    v.add(E >> alpha >>= I); // exposed becomes infectious
    v.add(I >> gamma >>= R); // infectious becomes removed
    v.add(I >> kappa >>= H); // infectious becomes hospitalized
    v.add(H >> tau >>= R); // hospitalized becomes removed
    return v;
}

int main() {
    uint32_t population = 10000; // Example population size

    auto state = std::make_shared<SystemState>();
    Vessel v = seihr(population, state);

    std::cerr << "Initial state: " << std::endl;
    state->prettyPrint();

    Simulator simulator(v, state, 100.0); // Run simulation for 100 time units

    std::vector<int> peakValues;
    simulator.run(); // Run 100 parallel simulations

    // Calculate the average peak value of the hospitalized population
    double averagePeak = std::accumulate(peakValues.begin(), peakValues.end(), 0.0) / peakValues.size();
    std::cout << "Average peak value of hospitalized population: " << averagePeak << std::endl;

    // Collect and plot the results using matplotlib
    const auto& results = state->getTrajectory();
    const auto& timePoints = state->getTimePoints();

    for (const auto& species : v.getSpecies()) {
        plt::named_plot(species.getName(), timePoints, results.at(species.getName()));
    }
    plt::legend();
    plt::show();

    // Generate the graph visualization
    GraphVisualizer gv(v.getSpecies(), v.getReactions());
    gv.generateGraph("reaction_graph.png");

    return 0;
}
