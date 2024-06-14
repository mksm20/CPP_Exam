#include "Simulator.h"
#include <algorithm>
#include <thread>
#include <functional>

Simulator::Simulator(const std::vector<Reaction>& reactions, SystemState& state, double endTime)
        : reactions(reactions), state(state), endTime(endTime), currentTime(0) {}

void Simulator::run() {
    while (currentTime < endTime) {
        handleReaction();
    }
}

void Simulator::handleReaction() {
    auto nextReaction = std::min_element(reactions.begin(), reactions.end(), [this](const Reaction& a, const Reaction& b) {
        return a.calculateDelay(state.getState()) < b.calculateDelay(state.getState());
    });

    double delay = nextReaction->calculateDelay(state.getState());
    currentTime += delay;
    nextReaction->execute(state.getState());
    state.recordState(currentTime);
}

void Simulator::runParallel(int numSimulations) {
    std::vector<std::thread> threads;
    for (int i = 0; i < numSimulations; ++i) {
        threads.emplace_back([this]() { this->run(); });
    }
    for (auto& thread : threads) {
        thread.join();
    }
}
