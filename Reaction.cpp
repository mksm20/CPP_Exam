//
// Created by martinmortensen on 6/14/24.
//

#include "Reaction.h"
#include <random>
#include <cmath>

Reaction::Reaction(const std::vector<std::string>& inputs, const std::vector<std::string>& outputs, double rate)
        : inputs(inputs), outputs(outputs), rate(rate) {}

double Reaction::calculateDelay(const std::map<std::string, int>& state) const {
    double product = rate;
    for (const auto& input : inputs) {
        product *= state.at(input);
    }
    std::exponential_distribution<double> distribution(product);
    std::random_device rd;
    std::mt19937 generator(rd());
    return distribution(generator);
}

void Reaction::execute(std::map<std::string, int>& state) const {
    for (const auto& input : inputs) {
        state[input]--;
    }
    for (const auto& output : outputs) {
        state[output]++;
    }
}
