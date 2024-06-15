#ifndef EXAM_LIB_SIMULATOR_H
#define EXAM_LIB_SIMULATOR_H
#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include "Reaction.h"
#include "SystemState.h"
#include "Vessel.h"
#include "Observer.h"

namespace sim {

    class Simulator {
    public:
        Simulator(const Vessel& vessel, std::shared_ptr<SystemState> state, double endTime, std::shared_ptr<Observer> observer = nullptr);
        Simulator(const Vessel vessel, SystemState state, double endTime, bool second_const, std::shared_ptr<Observer> observer = nullptr);
        void run();
        void runParallel(int numSimulations, std::vector<int>& peakValues, std::map<std::string, std::vector<double>>& aggregatedResults);

    private:
        std::vector<Reaction> reactions;
        std::shared_ptr<SystemState> state;
        Vessel vessel;
        double endTime;
        double currentTime;
        std::shared_ptr<Observer> observer; // Add observer member
        std::mutex mtx; // Mutex for thread-safe access

        void handleReaction();
        int runSingleSimulation(Vessel vessel, SystemState stateCopy, std::map<std::string, std::vector<double>>& aggregatedResults); // Helper function for running a single simulation
        int
        runSingleSimulation(Vessel vessel, SystemState stateCopy,
                            std::map<std::string, std::vector<double>> &aggregatedResults,
                            std::mutex &mtx);

        int
        runSingleSimulation(Vessel vessel, SystemState stateCopy,
                            std::map<std::string, std::vector<double>> &aggregatedResults,
                            std::mutex &mtx, std::stop_source);
    };

} // namespace sim

#endif //EXAM_LIB_SIMULATOR_H
