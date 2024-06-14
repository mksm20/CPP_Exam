#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"
#include "Simulator.h"
#include "SymbolTable.h"
#include "Observer.h"
#include <vector>
#include <iostream>
#include <map>
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    // Define species
    Species A("A", 100);
    Species B("B", 0);
    Species C("C", 1);

    // Define system state
    SystemState state;
    state.addSpecies(A);
    state.addSpecies(B);
    state.addSpecies(C);

    // Define reactions
    std::vector<Reaction> reactions = {
            Reaction({"A", "C"}, {"B", "C"}, 0.001)
    };

    // Create simulator
    Simulator simulator(reactions, state, 2000);

    // Run simulation
    simulator.run();

    // Print final state
    state.prettyPrint();

    // Plot results
    const auto& timePoints = state.getTimePoints();
    const auto& trajectory = state.getTrajectory();

    for (const auto& [species, counts] : trajectory) {
        // Convert counts to double
        std::vector<double> counts_double(counts.begin(), counts.end());
        plt::plot(timePoints, counts_double, {{"label", species}});
    }

    plt::xlabel("Time");
    plt::ylabel("Count");
    plt::legend();
    plt::show();

    return 0;
}
