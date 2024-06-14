//
// Created by martinmortensen on 6/14/24.
//

#ifndef EXAM_LIB_SIMULATOR_H
#define EXAM_LIB_SIMULATOR_H
#pragma once
#include <vector>
#include "Reaction.h"
#include "SystemState.h"
#include "Observer.h"

class Simulator {
public:
    Simulator(const std::vector<Reaction>& reactions, SystemState& state, double endTime);
    void run();
    void runParallel(int numSimulations);

private:
    std::vector<Reaction> reactions;
    SystemState& state;
    double endTime;
    double currentTime;
    void handleReaction();
};



#endif //EXAM_LIB_SIMULATOR_H
