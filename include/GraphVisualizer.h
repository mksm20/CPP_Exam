#ifndef EXAM_LIB_GRAPHVISUALIZER_H
#define EXAM_LIB_GRAPHVISUALIZER_H

#include <vector>
#include <string>
#include <graphviz/gvc.h>
#include "Species.h"
#include "Reaction.h"

namespace sim {
    class GraphVisualizer {
    public:
        GraphVisualizer(const std::vector<Species> &species, const std::vector<sim::Reaction> &reactions);

        void generateGraph(const std::string &filename);

    private:
        std::vector<Species> species;
        std::vector<sim::Reaction> reactions;
    };
}
#endif // EXAM_LIB_GRAPHVISUALIZER_H
