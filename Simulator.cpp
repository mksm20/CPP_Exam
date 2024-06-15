#include "Simulator.h"
#include <algorithm>
#include <functional>
#include <future>

namespace sim {

    Simulator::Simulator(const Vessel& vessel, std::shared_ptr<SystemState> state, double endTime)
            : vessel(const_cast<Vessel &>(vessel)) , reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0) {}

//Simulator::Simulator(Vessel vessel, SystemState state, double endTime, bool second_const)
  //          :vessel(vessel), reactions(vessel.getReactions(), state(state)){}

    void Simulator::run() {
        while (currentTime < endTime) {
            handleReaction();
        }
    }

    void Simulator::handleReaction() {
        // Calculate the delay for each reaction
        double minDelay = std::numeric_limits<double>::infinity();
        const Reaction* nextReaction = nullptr;

        for (const auto& reaction : reactions) {
            double delay = reaction.calculateDelay(state->getState());
            if (delay < minDelay) {
                minDelay = delay;
                nextReaction = &reaction;
            }
        }

        // Update current time with the delay of the next reaction
        currentTime += minDelay;

        // Execute the reaction
        if (nextReaction) {
            nextReaction->execute(state->getState(), *state);
        }

        // Record the state
        state->recordState(currentTime);
    }

    int Simulator::runSingleSimulation(Vessel vessel, SystemState stateCopy) {
        Simulator singleSimulator(vessel, std::make_shared<SystemState>(stateCopy), endTime);
        singleSimulator.run();

        // Find the peak value of the hospitalized population
        const auto& trajectory = stateCopy.getTrajectory();
        const auto& hospitalTrajectory = trajectory.at("H");
        int peakValue = *std::max_element(hospitalTrajectory.begin(), hospitalTrajectory.end());
        return peakValue;
    }

    void Simulator::runParallel(int numSimulations, std::vector<int>& peakValues) {
        std::vector<std::thread> threads;
        std::vector<std::future<int>> futures;
        for (int i = 0; i < numSimulations; ++i) {
            auto stateCopy = SystemState(*state); // Make a copy of the state for each thread
            auto vesselCopy = vessel; // Make a copy of the vessel for each thread
            std::promise<int> promise;
            futures.push_back(promise.get_future());
            threads.emplace_back([this, vesselCopy, stateCopy, &promise]() mutable {
                int peakValue = this->runSingleSimulation(vesselCopy, stateCopy);
                promise.set_value(peakValue);
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        for (auto& future : futures) {
            peakValues.push_back(future.get());
        }
    }

} // namespace sim
