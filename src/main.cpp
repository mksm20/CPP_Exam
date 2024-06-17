#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"
#include "Simulator.h"
#include "Observer.h"
#include "Vessel.h"
#include "GraphVisualizer.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <numeric>
#include <thread>
#include "../matplotlib-cpp/matplotlibcpp.h"
#include "PeakHospitalizationObserver.h"

namespace plt = matplotlibcpp;
using namespace sim;

void mltplt(std::shared_ptr<SystemState> systemState, const Vessel vessel,const std::vector<std::string> nameplots ,int scalar,const std::string species_name);

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

Vessel circadian_rhythm(std::shared_ptr<SystemState> state) {
    const auto alphaA = 50;
    const auto alpha_A = 500;
    const auto alphaR = 0.01;
    const auto alpha_R = 50;
    const auto betaA = 50;
    const auto betaR = 5;
    const auto gammaA = 1;
    const auto gammaR = 1;
    const auto gammaC = 2;
    const auto deltaA = 1;
    const auto deltaR = 0.2;
    const auto deltaMA = 10;
    const auto deltaMR = 0.5;
    const auto thetaA = 50;
    const auto thetaR = 100;


    auto v = Vessel{"Circadian Rhythm", state};
    const auto DA = v.add("DA", 1);
    const auto D_A = v.add("D_A", 0);
    const auto DR = v.add("DR", 1);
    const auto D_R = v.add("D_R", 0);
    const auto MA = v.add("MA", 0);
    const auto MR = v.add("MR", 0);
    const auto A = v.add("A", 0);
    const auto R = v.add("R", 0);
    const auto C = v.add("C", 0);

    v.add((A + DA) >> gammaA >>= D_A);
    v.add(D_A >> thetaA >>= DA + A);
    v.add((A + DR) >> gammaR >>= D_R);
    v.add(D_R >> thetaR >>= DR + A);
    v.add(D_A >> alpha_A >>= MA + D_A);
    v.add(DA >> alphaA >>= MA + DA);
    v.add(D_R >> alpha_R >>= MR + D_R);
    v.add(DR >> alphaR >>= MR + DR);
    v.add(MA >> betaA >>= MA + A);
    v.add(MR >> betaR >>= MR + R);
    v.add((A + R) >> gammaC >>= C);
    v.add(C >> deltaA >>= R);
    v.add(A >> deltaA >>= Species("env", 0, true));  // handle environment
    v.add(R >> deltaR >>= Species("env", 0, true));
    v.add(MA >> deltaMA >>= Species("env", 0, true));
    v.add(MR >> deltaMR >>= Species("env", 0, true));

    return v;
}
int main() {
    uint32_t population = 10000;
    // Create observer
    auto observer = std::make_shared<sim::PeakHospitalizationObserver>();

    // SEIHR Simulation
    auto state_seihr = std::make_shared<SystemState>();
    Vessel v_seihr = seihr(population, state_seihr);

    // Start observing


    std::cerr << "Initial state for SEIHR: " << std::endl;
    state_seihr->prettyPrint();

    // Pass observer to simulator
    Simulator simulator_seihr(v_seihr, state_seihr, 100.0, observer);
    observer->observe(state_seihr, 100, const_cast<double &>(simulator_seihr.getCurrentTime()));
    simulator_seihr.run();

    // ensure the observer's coroutine is completed
    while (observer->move_next());
    std::cout << "Peak Hospitalization: " << observer->getPeakHospitalization() << std::endl;
    for(const auto& species:v_seihr.getSpecies()){
        if(species.getName() == "H"){
            auto a = state_seihr->getTrajectory();
            auto int_vec = a.at("H");
            auto i = *max_element(int_vec.begin(), int_vec.end());
            std::cout << i << std::endl;

        }
    }


    //double averagePeak_seihr = std::accumulate(peakValues_seihr.begin(), peakValues_seihr.end(), 0.0) / peakValues_seihr.size();
    //std::cout << "Average peak value of hospitalized population (SEIHR): " << averagePeak_seihr << std::endl;

    // Collect and plot the results using matplotlib for SEIHR
    std::vector<std::string> nameplot{"S", "E", "I", "H", "R"};
    //std::jthread plot_thread(mltplt, state_seihr, v_seihr, nameplot, 1000, "H");
    mltplt(state_seihr, v_seihr, nameplot, 1000, "H");

    /*const auto& results_seihr = state_seihr->getTrajectory();
    const auto& timePoints_seihr = state_seihr->getTimePoints();

    for (const auto& species : v_seihr.getSpecies()) {
        if(species.getName() == "H") {
            auto results = results_seihr.at(species.getName());
            for(auto& value : results) {
                value *= 1000;
            }
            plt::named_plot(species.getName(), timePoints_seihr, results);
        }
        else
            plt::named_plot(species.getName(), timePoints_seihr, results_seihr.at(species.getName()));
    }
    plt::legend();
    plt::show();
    */
    // Generate the graph visualization for SEIHR
    GraphVisualizer gv_seihr(v_seihr.getSpecies(), v_seihr.getReactions());
    gv_seihr.generateGraph("seihr_graph.png");
    /*
    // Circadian Rhythm Simulation
    auto state_circadian = std::make_shared<SystemState>();
    Vessel v_circadian = circadian_rhythm(state_circadian);

    std::cerr << "Initial state for Circadian Rhythm: " << std::endl;
    state_circadian->prettyPrint();

    Simulator simulator_circadian(v_circadian, state_circadian, 100.0); 

    std::vector<int> peakValues_circadian;
    simulator_circadian.run(); 

    // Collect and plot the results using matplotlib for Circadian Rhythm
    const auto& results_circadian = state_circadian->getTrajectory();
    const auto& timePoints_circadian = state_circadian->getTimePoints();

    for (const auto& species : v_circadian.getSpecies()) {
        plt::named_plot(species.getName(), timePoints_circadian, results_circadian.at(species.getName()));
    }
    plt::legend();
    plt::show();

    // Generate the graph visualization for Circadian Rhythm
    GraphVisualizer gv_circadian(v_circadian.getSpecies(), v_circadian.getReactions());
    gv_circadian.generateGraph("circadian_rhythm_graph.png");
    */

    return 0;
}

void mltplt(std::shared_ptr<SystemState> systemState, const Vessel vessel,  const std::vector<std::string> nameplots,int scalar = 0, const std::string species_name = ""){
    const auto& results_traj = systemState->getTrajectory();
    const auto& timePoints_seihr = systemState->getTimePoints();

    for (const auto& species : vessel.getSpecies()) {
        if(std::find(nameplots.begin(), nameplots.end(), species.getName()) != nameplots.end()) {
            if (species.getName() == species_name) {
                auto results = results_traj.at(species.getName());
                for (auto &value: results) {
                    value *= scalar;
                }
                plt::named_plot(species.getName(), timePoints_seihr, results);
            } else
                plt::named_plot(species.getName(), timePoints_seihr, results_traj.at(species.getName()));
        }
    }
    plt::legend();
    plt::show();
}