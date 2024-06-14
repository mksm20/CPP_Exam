//
// Created by martinmortensen on 6/14/24.
//

#ifndef EXAM_LIB_SPECIES_H
#define EXAM_LIB_SPECIES_H


#pragma once
#include <string>

class Species {
public:
    Species(const std::string& name, int initialCount);
    const std::string& getName() const;
    int getCount() const;
    void setCount(int count);

private:
    std::string name;
    int count;
};



#endif //EXAM_LIB_SPECIES_H
