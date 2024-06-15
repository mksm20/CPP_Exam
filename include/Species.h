#ifndef EXAM_LIB_SPECIES_H
#define EXAM_LIB_SPECIES_H

#pragma once
#include <string>
#include <vector>

namespace sim {

    class CombinedSpecies; // Forward declaration

    class Species {
    public:
        Species();
        Species(const std::string& name, int initialCount);

        Species(const std::string &name, int initialCount, bool env);

        const std::string& getName() const;
        int getCount() const;
        void setCount(int count);
        bool isEnv() const;

        // Overloaded + operator
        CombinedSpecies operator+(const Species& other) const;

    private:
        std::string name;
        int count;
        bool env;
    };

    class CombinedSpecies {
    public:
        CombinedSpecies(const std::vector<Species>& species);
        const std::vector<Species>& getSpecies() const;

    private:
        std::vector<Species> species;
    };

// Declare the operators in the namespace
    class Reaction;

    Reaction operator>>(const CombinedSpecies& combined, double rate);
    Reaction operator>>(const CombinedSpecies& combined, const CombinedSpecies& output);

} // namespace sim

#endif //EXAM_LIB_SPECIES_H
