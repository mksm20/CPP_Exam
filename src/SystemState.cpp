#include "SystemState.h"
#include <iostream>
namespace sim {
    SystemState::SystemState(const SystemState& other)
            : state(other.state), trajectory(other.trajectory), timePoints(other.timePoints) {}

    void SystemState::addSpecies(const Species& species) {
        state.add(species.getName(), species.getCount());
        trajectory[species.getName()] = std::vector<int>();
    }

    int SystemState::getCount(const std::string& species) const {
        return state.lookup(species);
    }

    void SystemState::updateCount(const std::string& species, int count) {
        state.update(species, count);
    }

    void SystemState::prettyPrint() const {
        for (const auto& [name, count] : state.get_table()) {
            std::cout << name << ": " << count << std::endl;
        }
    }

    const std::map<std::string, int>& SystemState::getState() const {
        return state.get_table();
    }

    std::map<std::string, int>& SystemState::getState() {
        return state.get_table();
    }

    void SystemState::recordState(double time) {
        timePoints.push_back(time);
        for (const auto& [name, count] : state.get_table()) {
            if (trajectory.find(name) == trajectory.end()) {
                trajectory[name] = std::vector<int>();
            }
            trajectory[name].push_back(count);
        }
    }

    const std::map<std::string, std::vector<int>>& SystemState::getTrajectory() const {
        return trajectory;
    }

    const std::vector<double>& SystemState::getTimePoints() const {
        return timePoints;
    }

    void SystemState::replaceState() {
        SymbolTable<std::string, int> newTable(state); // Create copy symbol table to solve concurrency issue, does not solve anything
        state = newTable;
    }
    void SystemState::removeSpecies(const std::string& species, int count) {
        int currentCount = state.lookup(species);
        currentCount -= count;
        if (currentCount < 0) currentCount = 0;
        state.update(species, currentCount);
    }
}