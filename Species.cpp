#include "Species.h"

namespace sim {

    Species::Species() : name(""), count(0) {}

    Species::Species(const std::string& name, int initialCount)
            : name(name), count(initialCount), env(false){}
    Species::Species(const std::string& name, int initialCount, bool env)
            : name(name), count(initialCount), env(env) {}
    const std::string& Species::getName() const {
        return name;
    }

    int Species::getCount() const {
        return count;
    }

    void Species::setCount(int count) {
        this->count = count;
    }

    bool Species::isEnv() const {
        return env;
    }

    CombinedSpecies Species::operator+(const Species& other) const {
        std::vector<Species> combined_species = { *this, other };
        return CombinedSpecies(combined_species);
    }

    CombinedSpecies::CombinedSpecies(const std::vector<Species>& species) : species(species) {}

    const std::vector<Species>& CombinedSpecies::getSpecies() const {
        return species;
    }

} // namespace sim
