#include "Vessel.h"

namespace sim {

    Vessel::Vessel(const std::string& name, std::shared_ptr<SystemState> state) : name(name), state(state) {}

    Species Vessel::add(const std::string& name, int initialCount) {
        Species s(name, initialCount);
        species.push_back(s);
        speciesMap[name] = s;
        state->addSpecies(s); // Add species to the state
        return s;
    }

    void Vessel::add(const Reaction& reaction) {
        reactions.push_back(reaction);
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
