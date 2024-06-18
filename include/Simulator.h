#ifndef EXAM_LIB_SIMULATOR_H
#define EXAM_LIB_SIMULATOR_H
#pragma once
#include "Vessel.h"
#include "SystemState.h"
#include "Observer.h"
#include <vector>
#include <memory>
#include <coroutine>
#include <map>
#include <mutex>

namespace sim {

    class Simulator {
    public:
        Simulator(const Vessel& vessel, SystemState &state, double endTime);
        Simulator(Vessel &vessel, SystemState &state, double endTime);
        Simulator(const Vessel& vessel, SystemState &state, double endTime, std::shared_ptr<Observer> observer);
        Simulator(Vessel vessel, SystemState state, double endTime, bool second_const, std::shared_ptr<Observer> observer);
        static bool available();
        void run();
        void runParallel(int numSimulations, std::map<std::string, std::vector<double>>& peakValues, std::map<std::string, std::vector<double>>& aggregatedResults, std::vector<SystemState> &states, std::vector<Vessel> &vessels);
        const double &getCurrentTime(){return currentTime;}
    private:
        void handleReaction();
        std::map<std::string, std::vector<double>> runSingleSimulation(Vessel vessel, SystemState stateCopy, std::map<std::string, std::vector<double>>& aggregatedResults);

        Vessel vessel;
        std::vector<Reaction> reactions;
        SystemState &state;
        double endTime;
        double currentTime;
        std::shared_ptr<Observer> observer;
    };

}

#endif // EXAM_LIB_SIMULATOR_H
