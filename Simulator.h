#ifndef EXAM_LIB_SIMULATOR_H
#define EXAM_LIB_SIMULATOR_H
#pragma once
#include <vector>
#include "Reaction.h"
#include "SystemState.h"
#include "Observer.h"
#include "Vessel.h"
namespace sim {
    class Simulator {
    public:
        Simulator(const Vessel &vessel, SystemState &state, double endTime);

        void run();

        void runParallel(int numSimulations);

    private:
        std::vector<Reaction> reactions;
        SystemState &state;
        double endTime;
        double currentTime;

        void handleReaction();
    };
}
#endif //EXAM_LIB_SIMULATOR_H
