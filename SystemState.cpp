//
// Created by martinmortensen on 6/14/24.
//
#include "SystemState.h"
#include <iostream>
namespace sim {
    void SystemState::addSpecies(const Species &species) {
        state[species.getName()] = species.getCount();
    }

    void SystemState::ensureSpecies(const Species& species) {
        if (state.find(species.getName()) == state.end()) {
            addSpecies(species);
        }
    }

    int SystemState::getCount(const std::string &species) const {
        return state.at(species);
    }

    void SystemState::updateCount(const std::string &species, int count) {
        state[species] = count;
    }

    void SystemState::prettyPrint() const {
        for (const auto &[name, count]: state) {
            std::cout << name << ": " << count << std::endl;
        }
    }

    const std::map<std::string, int> &SystemState::getState() const {
        return state;
    }

    std::map<std::string, int> &SystemState::getState() {
        return state;
    }

    void SystemState::recordState(double time) {
        timePoints.push_back(time);
        for (const auto &[name, count]: state) {
            trajectory[name].push_back(count);
        }
    }

    const std::map<std::string, std::vector<int>> &SystemState::getTrajectory() const {
        return trajectory;
    }

    const std::vector<double> &SystemState::getTimePoints() const {
        return timePoints;
    }
}