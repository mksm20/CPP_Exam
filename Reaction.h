//
// Created by martinmortensen on 6/14/24.
//

#ifndef EXAM_LIB_REACTION_H
#define EXAM_LIB_REACTION_H


#pragma once
#include <vector>
#include <random>
#include <string>
#include <map>

class Reaction {
public:
    Reaction(const std::vector<std::string>& inputs, const std::vector<std::string>& outputs, double rate);
    double calculateDelay(const std::map<std::string, int>& state) const;
    void execute(std::map<std::string, int>& state) const;

private:
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    double rate;
};



#endif //EXAM_LIB_REACTION_H
