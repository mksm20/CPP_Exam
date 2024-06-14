//
// Created by martinmortensen on 6/14/24.
//

#include "Species.h"

Species::Species(const std::string& name, int initialCount)
        : name(name), count(initialCount) {}

const std::string& Species::getName() const {
    return name;
}

int Species::getCount() const {
    return count;
}

void Species::setCount(int count) {
    this->count = count;
}

