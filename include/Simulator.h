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
        Simulator(const Vessel& vessel, std::shared_ptr<SystemState> state, double endTime, std::shared_ptr<Observer> observer);
        Simulator(Vessel vessel, SystemState state, double endTime, bool second_const, std::shared_ptr<Observer> observer);

        void run();
        void runParallel(int numSimulations, std::vector<int>& peakValues, std::map<std::string, std::vector<double>>& aggregatedResults);
        const double &getCurrentTime(){return currentTime;}
    private:
        void handleReaction();
        int runSingleSimulation(Vessel vessel, SystemState stateCopy, std::map<std::string, std::vector<double>>& aggregatedResults, std::mutex& mtx);

        Vessel vessel;
        std::vector<Reaction> reactions;
        std::shared_ptr<SystemState> state;
        double endTime;
        double currentTime;
        std::shared_ptr<Observer> observer;
    };

}

#endif // EXAM_LIB_SIMULATOR_H
