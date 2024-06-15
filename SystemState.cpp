#include "SystemState.h"
#include <iostream>
namespace sim {
    void SystemState::addSpecies(const Species& species) {
        stateTable.add(species.getName(), species.getCount());
    }

    int SystemState::getCount(const std::string& species) const {
        return stateTable.lookup(species);
    }

    void SystemState::updateCount(const std::string& species, int count) {
        stateTable.update(species, count);
    }

    void SystemState::removeSpecies(const std::string& species, int count) {
        int currentCount = stateTable.lookup(species);
        currentCount -= count;
        if (currentCount < 0) currentCount = 0;
        stateTable.update(species, currentCount);
    }

    void SystemState::prettyPrint() const {
        for (const auto& [name, count] : stateTable.getTable()) {
            std::cout << name << ": " << count << std::endl;
        }
    }

    const std::map<std::string, int>& SystemState::getState() const {
        return stateTable.getTable();
    }

    std::map<std::string, int>& SystemState::getState() {
        return const_cast<std::map<std::string, int> &>(stateTable.getTable());
    }

    void SystemState::recordState(double time) {
        timePoints.push_back(time);
        for (const auto& [name, count] : stateTable.getTable()) {
            trajectory[name].push_back(count);
        }
    }

    const std::map<std::string, std::vector<int>>& SystemState::getTrajectory() const {
        return trajectory;
    }

    const std::vector<double>& SystemState::getTimePoints() const {
        return timePoints;
    }

}