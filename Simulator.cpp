#include "Simulator.h"
#include <algorithm>
#include <thread>
#include <functional>
namespace sim {
    Simulator::Simulator(const Vessel &vessel, SystemState &state, double endTime)
            : reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0) {
        // Initialize the system state with species from the vessel
        for (const auto &species: vessel.getSpecies()) {
            state.addSpecies(species);
        }
    }

    void Simulator::run() {
        while (currentTime < endTime) {
            handleReaction();
        }
    }

    void Simulator::handleReaction() {
        // Calculate the delay for each reaction
        double minDelay = std::numeric_limits<double>::infinity();
        const Reaction *nextReaction = nullptr;

        for (const auto &reaction: reactions) {
            double delay = reaction.calculateDelay(state.getState());
            if (delay < minDelay) {
                minDelay = delay;
                nextReaction = &reaction;
            }
        }

        // Update current time with the delay of the next reaction
        currentTime += minDelay;

        // Execute the reaction
        if (nextReaction) {
            nextReaction->execute(state.getState());
        }

        // Record the state
        state.recordState(currentTime);
    }

    void Simulator::runParallel(int numSimulations) {
        std::vector<std::thread> threads;
        for (int i = 0; i < numSimulations; ++i) {
            threads.emplace_back([this]() { this->run(); });
        }
        for (auto &thread: threads) {
            thread.join();
        }
    }
}