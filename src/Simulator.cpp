#include "Simulator.h"
#include "PeakHospitalizationObserver.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <future>
#include <random>
#include <condition_variable>
#include "functions.h"

namespace sim {
    std::mutex mtx;

    Simulator::Simulator(const Vessel& vessel, SystemState &state, double endTime)
            : vessel(vessel), reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0) {}

    Simulator::Simulator(Vessel &vessel, SystemState &state, double endTime)
            : vessel(vessel), reactions(vessel.getReactions()), state(state), currentTime(0), endTime(endTime) {}

    Simulator::Simulator(const Vessel& vessel, SystemState &state, double endTime, std::shared_ptr<Observer> observer)
            : vessel(const_cast<Vessel &>(vessel)), reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0), observer(observer) {}

    Simulator::Simulator(Vessel vessel, SystemState state, double endTime, bool second_const, std::shared_ptr<Observer> observer)
            : vessel(vessel), reactions(vessel.getReactions()), state(state), endTime(endTime), currentTime(0), observer(observer) {}

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
            nextReaction->execute(state.getState(), state);
        }

        // Use the observer to observe the current state
        if (observer) {
            observer->move_next();
        }

        // Record the state
        state.recordState(currentTime);
    }

    std::map<std::string, std::vector<double>> Simulator::runSingleSimulation(Vessel vessel, SystemState stateCopy, std::map<std::string, std::vector<double>>& aggregatedResults) {
        //std::cout << "Starting single simulation" << std::endl;
        std::map<std::string, std::vector<double>> res;
        auto s = std::make_unique<SystemState>();
        auto v = std::make_unique<Vessel>(seihr(100000, *s));
        auto singleSimulator = std::make_unique<Simulator>(*v, *s, endTime);

        std::cout << v.get() << " " << s.get() << " " << singleSimulator.get() << " Addresses of vessel, state, and simulator" << std::endl;

        singleSimulator->run();
        //std::scoped_lock lock(mtx);

        const auto &trajectory = singleSimulator->state.getTrajectory();
        for (const auto &[species, counts]: trajectory) {
            if (res.find(species) == res.end()) {
                res[species].resize(counts.size(), 0.0);
            }
            for (size_t i = 0; i < counts.size(); ++i) {
                res[species][i] += counts[i];
            }
        }

        return res;
    }


    void Simulator::runParallel(int numSimulations, std::map<std::string, std::vector<double>>& peakValues, std::map<std::string, std::vector<double>>& aggregatedResults, std::vector<SystemState> &states, std::vector<Vessel> &vessels) {
        std::vector<std::jthread> threads;
        std::vector<std::promise<std::map<std::string, std::vector<double>>>> promises(numSimulations); // N is the size of the vector

        std::vector<std::future<std::map<std::string, std::vector<double>>>> futures;

        for (int i = 0; i < numSimulations; ++i) {
            auto vesselCopy = vessels.at(i);
            promises[i] = std::promise<std::map<std::string, std::vector<double>>>();
            auto state = vesselCopy.getSystemState();
            futures.push_back(promises[i].get_future());
            std::cout << "Starting simulation thread " << i + 1 << " out of " << numSimulations << std::endl;
            threads.emplace_back([this, vessels, state, promise = std::move(promises[i]), &aggregatedResults, i]() mutable {
                try {
                    auto state = SystemState();
                    std::map<std::string, std::vector<double>> peakValue = this->runSingleSimulation(vessels.at(i), vessels.at(i).getSystemState(), aggregatedResults);
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
                auto result = future.get();
                for (const auto& [species, counts] : result) {
                    if (aggregatedResults.find(species) == aggregatedResults.end()) {
                        aggregatedResults[species].resize(counts.size(), 0.0);
                    }
                    for (size_t i = 0; i < counts.size(); ++i) {
                        aggregatedResults[species][i] += counts[i];
                    }
                }
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
