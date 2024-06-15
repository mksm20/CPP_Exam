#include "../include/Reaction.h"
#include "Species.h"
#include <random>
#include <cmath>
#include <iostream>

namespace sim {

    Reaction::Reaction(const std::vector<std::string>& inputs, const std::vector<std::string>& outputs, double rate)
            : inputs(inputs), outputs(outputs), rate(rate) {
        std::cerr << "Created reaction with inputs: ";
        for (const auto& input : inputs) std::cerr << input << " ";
        std::cerr << "and outputs: ";
        for (const auto& output : outputs) std::cerr << output << " ";
        std::cerr << "at rate: " << rate << std::endl;
    }

    double Reaction::calculateDelay(const std::map<std::string, int>& state) const {
        double product = rate;
        for (const auto& input : inputs) {
            if (state.find(input) == state.end()) {
                std::cerr << "Error: Key " << input << " not found in state map." << std::endl;
                throw std::out_of_range("Key not found in state map.");
            }
            product *= state.at(input);
        }
        std::exponential_distribution<double> distribution(product);
        std::random_device rd;
        std::mt19937 generator(rd());
        double delay = distribution(generator);
        //std::cerr << "Calculated delay for reaction: " << delay << std::endl;
        return delay;
    }

    void Reaction::execute(std::map<std::string, int>& state, SystemState& fullState) const {
        for (const auto& input : inputs) {
            if (input != "env") {
                state[input]--;
            } else {
                fullState.removeSpecies(input, 1);
            }
        }
        for (const auto& output : outputs) {
            if (output != "env") {
                state[output]++;
            }
        }
    }

    void Reaction::executeForEnvironment(SystemState& state) const {
        for (const auto& input : inputs) {
            state.removeSpecies(input, 1);
        }
    }

    const std::vector<std::string>& Reaction::getInputs() const {
        return inputs;
    }

    const std::vector<std::string>& Reaction::getOutputs() const {
        return outputs;
    }

    const double Reaction::getRate() const {
        return rate;
    }

    Reaction operator>>(const Species& input, const Species& output) {
        return Reaction({input.getName()}, {output.getName()}, 0.0);
    }

    Reaction operator>>(const std::vector<Species>& inputs, const Species& output) {
        std::vector<std::string> inputNames;
        for (const auto& input : inputs) {
            inputNames.push_back(input.getName());
        }
        return Reaction(inputNames, {output.getName()}, 0.0);
    }

    Reaction operator>>(const Species& input, double rate) {
        return Reaction({input.getName()}, {}, rate);
    }

    Reaction operator>>(const std::vector<Species>& inputs, double rate) {
        std::vector<std::string> inputNames;
        for (const auto& input : inputs) {
            inputNames.push_back(input.getName());
        }
        return Reaction(inputNames, {}, rate);
    }

    Reaction operator>>(const CombinedSpecies& combined, double rate) {
        std::vector<std::string> inputNames;
        for (const auto& input : combined.getSpecies()) {
            inputNames.push_back(input.getName());
        }
        return Reaction(inputNames, {}, rate);
    }

    Reaction operator>>(const CombinedSpecies& combined, const Species& output) {
        std::vector<std::string> inputNames;
        for (const auto& input : combined.getSpecies()) {
            inputNames.push_back(input.getName());
        }
        return Reaction(inputNames, {output.getName()}, 0.0);
    }

    Reaction operator>>(const CombinedSpecies& combined, const CombinedSpecies& output) {
        std::vector<std::string> inputNames;
        std::vector<std::string> outputNames;
        for (const auto& input : combined.getSpecies()) {
            inputNames.push_back(input.getName());
        }
        for (const auto& out : output.getSpecies()) {
            outputNames.push_back(out.getName());
        }
        return Reaction(inputNames, outputNames, 0.0);
    }

    Reaction Reaction::operator>>=(double rate) const {
        return Reaction(inputs, outputs, rate);
    }

    Reaction Reaction::operator>>=(const Species& output) const {
        std::vector<std::string> newOutputs = outputs;
        newOutputs.push_back(output.getName());
        return Reaction(inputs, newOutputs, rate);
    }

    Reaction Reaction::operator>>=(const CombinedSpecies& outputs) const {
        std::vector<std::string> newOutputs = this->outputs;
        for (const auto& output : outputs.getSpecies()) {
            newOutputs.push_back(output.getName());
        }
        return Reaction(inputs, newOutputs, rate);
    }

} // namespace sim
