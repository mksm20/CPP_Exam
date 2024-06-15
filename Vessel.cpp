#include "Vessel.h"
#include <iostream>

namespace sim {

    Vessel::Vessel(const std::string& name, SystemState& state) : name(name), state(state) {}

    Species Vessel::add(const std::string& name, int initialCount) {
        Species s(name, initialCount);
        species.push_back(s);
        state.addSpecies(s); // Add to the system state
        return s;
    }

    void Vessel::add(const Reaction& reaction) {
        reactions.push_back(reaction);

        // Ensure all inputs and outputs are initialized in the system state
        for (const auto& input : reaction.getInputs()) {
            state.ensureSpecies(Species(input, 0));
        }
        for (const auto& output : reaction.getOutputs()) {
            state.ensureSpecies(Species(output, 0));
        }
    }

    const std::string& Vessel::getName() const {
        return name;
    }

    const std::vector<Species>& Vessel::getSpecies() const {
        return species;
    }

    const std::vector<Reaction>& Vessel::getReactions() const {
        return reactions;
    }

} // namespace sim
