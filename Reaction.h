#ifndef EXAM_LIB_REACTION_H
#define EXAM_LIB_REACTION_H
#pragma once
#include <vector>
#include <string>
#include <map>
#include "Species.h"

namespace sim {

// Forward declaration of Species and CombinedSpecies classes
    class Species;
    class CombinedSpecies;

    class Reaction {
    public:
        Reaction(const std::vector<std::string>& inputs, const std::vector<std::string>& outputs, double rate);
        double calculateDelay(const std::map<std::string, int>& state) const;
        void execute(std::map<std::string, int>& state) const;

        // Overloaded >>= operator for reaction rate
        Reaction operator>>=(double rate) const;
        Reaction operator>>=(const Species& output) const;
        Reaction operator>>=(const CombinedSpecies& outputs) const;

        // Getters for inputs and outputs
        const std::vector<std::string>& getInputs() const;
        const std::vector<std::string>& getOutputs() const;

    private:
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
        double rate;
    };

// Overloaded operators for reaction process and rate
    Reaction operator>>(const Species& input, const Species& output);
    Reaction operator>>(const std::vector<Species>& inputs, const Species& output);
    Reaction operator>>(const Species& input, double rate);
    Reaction operator>>(const std::vector<Species>& inputs, double rate);
    Reaction operator>>(const CombinedSpecies& combined, double rate);
    Reaction operator>>(const CombinedSpecies& combined, const Species& output);
    Reaction operator>>(const CombinedSpecies& combined, const CombinedSpecies& output);

} // namespace sim

#endif //EXAM_LIB_REACTION_H
