#include <iostream>
#include <chrono>
#include "Simulator.h"
#include <cmath>
#include "SystemState.h"
#include "Vessel.h"
#include "functions.h"

using namespace sim;

void benchmarkSingleCore(int numSimulations, double endTime) {

    auto state = sim::SystemState();
    sim::Vessel vessel = circadian_rhythm(state);
    sim::Simulator simulator(vessel, state, endTime);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numSimulations; ++i) {
        auto stateCopy = state;
        sim::Simulator singleSimulator(vessel, stateCopy, endTime);
        singleSimulator.run();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken for " << numSimulations << " simulations on a single core: "
              << duration.count() << " seconds." << std::endl;
}

void benchmarkmultiCore(int numSimulations,int threads ,double endTime) {

    auto state = SystemState();
    sim::Vessel vessel = circadian_rhythm(state);
    sim::Simulator simulator(vessel, state, endTime);
    std::vector<Vessel> vessels;
    std::vector<SystemState> states;
    for(auto i = 0;i < 10; i++){
        auto stat = state;
        auto v= vessel;
        states.push_back(stat);
        vessels.push_back(v);
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::basic_string<char>,std::vector<double>> aggregated_results;
    std::vector<int> peakvalues;
    for (int i = 0; i < numSimulations; ++i) {
        auto stateCopy = state;
        sim::Simulator singleSimulator(vessel, stateCopy, endTime);
        singleSimulator.runParallel(threads, (std::vector<int> &) peakvalues,
                                    (std::map<std::string, std::vector<double>> &) aggregated_results,
                                    (std::vector<SystemState> &) states, (std::vector<Vessel> &) vessels);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken for " << numSimulations << " simulations on a single core: "
              << duration.count() << " seconds." << std::endl;
}

int main() {
    int numSimulations = 5;
    double endTime = 100.0;
    int numberThread = 10;

    //benchmarkSingleCore(numSimulations, endTime);
    benchmarkmultiCore(1, numberThread, endTime );

    return 0;
}
