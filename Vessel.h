#ifndef EXAM_LIB_VESSEL_H
#define EXAM_LIB_VESSEL_H
#include <vector>
#include <string>
#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"

namespace sim {

    class Vessel {
    public:
        Vessel(const std::string& name, SystemState& state);
        Species add(const std::string& name, int initialCount);
        void add(const Reaction& reaction);
        const std::string& getName() const;
        const std::vector<Species>& getSpecies() const;
        const std::vector<Reaction>& getReactions() const;

    private:
        std::string name;
        std::vector<Species> species;
        std::vector<Reaction> reactions;
        SystemState& state;
    };

} // namespace sim

#endif //EXAM_LIB_VESSEL_H
