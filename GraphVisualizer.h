#ifndef EXAM_LIB_GRAPHVISUALIZER_H
#define EXAM_LIB_GRAPHVISUALIZER_H

#include <string>
#include <vector>
#include "Reaction.h"
#include "Species.h"

namespace sim {

    class GraphVisualizer {
    public:
        GraphVisualizer(const std::vector<Species>& species, const std::vector<Reaction>& reactions);
        void generateGraph(const std::string& filename);

    private:
        std::vector<Species> species;
        std::vector<Reaction> reactions;
    };

} // namespace sim

#endif //EXAM_LIB_GRAPHVISUALIZER_H
