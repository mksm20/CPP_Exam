#ifndef EXAM_LIB_SIMULATOR_H
#define EXAM_LIB_SIMULATOR_H
#pragma once
#include <vector>
#include <thread>
#include <memory>
#include "Reaction.h"
#include "SystemState.h"
#include "Vessel.h"

namespace sim {

    class Simulator {
    public:
        Simulator(const Vessel& vessel, std::shared_ptr<SystemState> state, double endTime);
        Simulator(Vessel vessel, SystemState state, double endTime);

        void run();
        void runParallel(int numSimulations, std::vector<int>& peakValues);

    private:
        std::vector<Reaction> reactions;
        std::shared_ptr<SystemState> state;
        Vessel vessel;
        double endTime;
        double currentTime;
        void handleReaction();
        int runSingleSimulation(Vessel vessel, SystemState stateCopy); // Helper function for running a single simulation
    };

} // namespace sim

#endif //EXAM_LIB_SIMULATOR_H
