#include "Simulator.h"
#include "PeakHospitalizationObserver.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <future>

namespace sim {

    Simulator::Simulator(const Vessel& vessel, std::shared_ptr<SystemState> state, double endTime, std::shared_ptr<Observer> observer)
            : vessel(const_cast<Vessel &>(vessel)), reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0), observer(observer) {}

    Simulator::Simulator(Vessel vessel, SystemState state, double endTime, bool second_const, std::shared_ptr<Observer> observer)
            : vessel(vessel), reactions(vessel.getReactions()), state(std::make_shared<SystemState>(state)), endTime(endTime), currentTime(0), observer(observer) {}

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

        // Use the observer to observe the current state
        if (observer) {
            observer->move_next();
        }

        // Record the state
        state->recordState(currentTime);
    }

    int Simulator::runSingleSimulation(Vessel vessel, SystemState stateCopy, std::map<std::string, std::vector<double>>& aggregatedResults, std::mutex& mtx) {
        std::cout << "Starting single simulation" << std::endl;
        auto localObserver = std::make_shared<PeakHospitalizationObserver>(); // Use the concrete observer class
        Simulator singleSimulator(vessel, std::make_shared<SystemState>(stateCopy), endTime, localObserver);
        singleSimulator.run();

        std::cout << "Single simulation run completed" << std::endl;

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Aggregating results" << std::endl;
            const auto& trajectory = stateCopy.getTrajectory();
            std::cout << "Trajectory size: " << trajectory.size() << std::endl;
            for (const auto& [species, counts] : trajectory) {
                std::cout << "Species: " << species << " Counts size: " << counts.size() << std::endl;
                if (aggregatedResults.find(species) == aggregatedResults.end()) {
                    aggregatedResults[species].resize(counts.size(), 0.0);
                }
                for (size_t i = 0; i < counts.size(); ++i) {
                    aggregatedResults[species][i] += counts[i];
                }
                std::cout << "Results aggregated for species: " << species << std::endl;
            }
        }

        // Find the peak value of the hospitalized population
        int peakHospitalization = localObserver->getPeakHospitalization();
        std::cout << "Peak hospitalization: " << peakHospitalization << std::endl;
        return peakHospitalization;
    }

    void Simulator::runParallel(int numSimulations, std::vector<int>& peakValues, std::map<std::string, std::vector<double>>& aggregatedResults) {
        std::vector<std::jthread> threads;
        std::vector<std::promise<int>> promises(numSimulations);
        std::vector<std::future<int>> futures;
        std::mutex mtx;

        for (int i = 0; i < numSimulations; ++i) {
            auto stateCopy = SystemState(*state); // Make a copy of the state for each thread
            stateCopy.replaceState(); // Copy symbol table by value
            auto vesselCopy = vessel; // Make a copy of the vessel for each thread
            promises[i] = std::promise<int>();
            futures.push_back(promises[i].get_future());
            std::cout << "Starting simulation thread " << i + 1 << std::endl;
            threads.emplace_back([this, vesselCopy, stateCopy, promise = std::move(promises[i]), &aggregatedResults, &mtx]() mutable {
                try {
                    int peakValue = this->runSingleSimulation(vesselCopy, stateCopy, aggregatedResults, mtx);
                    std::cout << "Simulation thread completed with peak value: " << peakValue << std::endl;
                    promise.set_value(peakValue);
                } catch (const std::exception& e) {
                    std::cerr << "Exception in thread: " << e.what() << std::endl;
                    promise.set_exception(std::current_exception());
                } catch (...) {
                    std::cerr << "Unknown exception in thread" << std::endl;
                    promise.set_exception(std::current_exception());
                }
            });
        }
        for (auto& future : futures) {
            try {
                peakValues.push_back(future.get());
            } catch (const std::exception& e) {
                std::cerr << "Exception while getting future: " << e.what() << std::endl;
            }
        }

        // Normalize results
        for (auto& [species, counts] : aggregatedResults) {
            for (auto& count : counts) {
                count /= numSimulations;
            }
        }
        std::cout << "Completed all parallel simulations" << std::endl;
    }

}
