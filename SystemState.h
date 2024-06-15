//
// Created by martinmortensen on 6/14/24.
//

#ifndef EXAM_LIB_SYSTEMSTATE_H
#define EXAM_LIB_SYSTEMSTATE_H
#pragma once
#include <map>
#include <string>
#include <vector>
#include "Species.h"
namespace sim {
    class SystemState {
    public:
        void addSpecies(const Species& species);
        void ensureSpecies(const Species& species);
        int getCount(const std::string& species) const;
        void updateCount(const std::string& species, int count);
        void prettyPrint() const;
        const std::map<std::string, int>& getState() const;
        std::map<std::string, int>& getState();
        void recordState(double time);
        const std::map<std::string, std::vector<int>>& getTrajectory() const;
        const std::vector<double>& getTimePoints() const;

    private:
        std::map<std::string, int> state;
        std::map<std::string, std::vector<int>> trajectory;
        std::vector<double> timePoints;
    };
}



#endif //EXAM_LIB_SYSTEMSTATE_H
