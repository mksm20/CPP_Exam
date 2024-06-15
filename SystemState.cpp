#include "SystemState.h"
#include <iostream>
namespace sim {
    void SystemState::addSpecies(const Species& species) {
        speciesTable.add(species.getName(), species.getCount());
        state[species.getName()] = species.getCount();
        std::cerr << "Added species " << species.getName() << " with count " << species.getCount() << std::endl;
    }

    void SystemState::ensureSpecies(const Species& species) {
        try {
            speciesTable.lookup(species.getName());
        } catch (const std::runtime_error&) {
            addSpecies(species);
        }
    }

    int SystemState::getCount(const std::string& species) const {
        return speciesTable.lookup(species);
    }

    void SystemState::updateCount(const std::string& species, int count) {
        if (state.find(species) == state.end()) {
            throw std::runtime_error("Species not found in state");
        }
        speciesTable.update(species, count); // Update the symbol table
        state[species] = count;
        //std::cerr << "Updated " << species << " to count " << count << std::endl;
    }

    void SystemState::prettyPrint() const {
        for (const auto& [name, count] : state) {
            std::cout << name << ": " << count << std::endl;
        }
    }

    const std::map<std::string, int>& SystemState::getState() const {
        return state;
    }

    std::map<std::string, int>& SystemState::getState() {
        return state;
    }

    void SystemState::recordState(double time) {
        timePoints.push_back(time);
        //std::cerr << "Recording state at time " << time << std::endl;
        for (const auto& [name, count] : state) {
            trajectory[name].push_back(count);
            //std::cerr << "  " << name << ": " << count << std::endl;
        }
    }

    const std::map<std::string, std::vector<int>>& SystemState::getTrajectory() const {
        return trajectory;
    }

    const std::vector<double>& SystemState::getTimePoints() const {
        return timePoints;
    }
}