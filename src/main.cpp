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
#include <iomanip>
#include "functions.h"
#include "../matplotlib-cpp/matplotlibcpp.h"
#include "PeakHospitalizationObserver.h"

namespace plt = matplotlibcpp;
using namespace sim;

void mltplt(std::shared_ptr<SystemState> systemState, const Vessel vessel,const std::vector<std::string> nameplots ,int scalar,const std::string species_name);
void displayAggregatedResults(const std::map<std::string, std::vector<double>> &aggregatedResults) ;
void plotAggregatedResults(const std::map<std::string, std::vector<double>> &aggregatedResults, const std::vector<double> &timePoints, const std::vector<std::string> &nameplots, int scalar = 0, const std::string &species_name = "");



int main() {
    uint32_t population = 10000;
    // Create observer
    auto observer = std::make_shared<sim::PeakHospitalizationObserver>();
    // SEIHR Simulation
    auto state_seihr = SystemState();
    auto v_seihr = seihr(population, state_seihr);
    std::vector<Vessel> vessels;
    std::vector<SystemState> states;
    for(auto i = 0;i < 10; i++){
        auto state = v_seihr.getSystemState();
        auto v= v_seihr;
        states.push_back(state);
        vessels.push_back(v);
    }

    // Start observing


    std::cerr << "Initial state for SEIHR: " << std::endl;
    state_seihr.prettyPrint();
    std::vector<int>peak_values{};
    std::map<std::basic_string<char>,std::vector<double>> aggregated_results;

    // Pass observer to simulator
    Simulator simulator_seihr(v_seihr, state_seihr, 100.0, observer);
    //observer->observe(state_seihr, 100, const_cast<double &>(simulator_seihr.getCurrentTime()));
    simulator_seihr.runParallel(6, peak_values, aggregated_results, (std::vector<SystemState> &) states,
                                (std::vector<Vessel> &) vessels);


    // ensure the observer's coroutine is completed
    while (observer->move_next());
    std::cout << "Peak Hospitalization: " << observer->getPeakHospitalization() << std::endl;
    std::cout << "getting to aggregated results" << std::endl;
    displayAggregatedResults(aggregated_results);


    //double averagePeak_seihr = std::accumulate(peakValues_seihr.begin(), peakValues_seihr.end(), 0.0) / peakValues_seihr.size();
    //std::cout << "Average peak value of hospitalized population (SEIHR): " << averagePeak_seihr << std::endl;

    // Collect and plot the results using matplotlib for SEIHR
    std::vector<std::string> nameplot{"S", "E", "I", "H", "R"};
    //std::jthread plot_thread(mltplt, state_seihr, v_seihr, nameplot, 1000, "H");
    //mltplt(state_seihr, v_seihr, nameplot, 1000, "H");

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

void displayAggregatedResults(const std::map<std::string, std::vector<double>> &aggregatedResults) {
    std::cout << "\nAggregated Results:\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << std::setw(15) << "Species" << std::setw(15) << "Count Index" << std::setw(15) << "Count Value" << "\n";
    std::cout << "--------------------------------------------------\n";
    auto count_avg = 0;
    for (const auto &[species, counts] : aggregatedResults) {
        count_avg = 0;
        for (size_t i = 0; i < counts.size(); ++i) {
            count_avg += counts[i];
            count_avg/= 2;
        }
        std::cout << std::setw(15) << species << std::setw(15) << std::setw(15) << count_avg << "\n";
        std::cout << "--------------------------------------------------\n";
    }
}

void mltplt(SystemState &systemState, const Vessel vessel,  const std::vector<std::string> nameplots,int scalar = 0, const std::string species_name = ""){
    const auto& results_traj = systemState.getTrajectory();
    const auto& timePoints_seihr = systemState.getTimePoints();

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

