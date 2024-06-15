#include "GraphVisualizer.h"
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

namespace sim {

    GraphVisualizer::GraphVisualizer(const std::vector<Species>& species, const std::vector<Reaction>& reactions)
            : species(species), reactions(reactions) {}

    void GraphVisualizer::generateGraph(const std::string& filename) {
        GVC_t *gvc = gvContext();
        Agraph_t *g = agopen((char*)"g", Agdirected, NULL);

        // Create nodes for species
        std::map<std::string, Agnode_t*> speciesNodes;
        for (const auto& s : species) {
            Agnode_t *node = agnode(g, (char*)s.getName().c_str(), 1);
            agsafeset(node, (char*)"shape", (char*)"box", (char*)"");
            agsafeset(node, (char*)"style", (char*)"filled", (char*)"");
            agsafeset(node, (char*)"fillcolor", (char*)"cyan", (char*)"");
            speciesNodes[s.getName()] = node;
        }

        // Create nodes and edges for reactions
        int reactionCount = 0;
        for (const auto& r : reactions) {
            std::string reactionLabel = std::to_string(r.getRate());
            Agnode_t *reactionNode = agnode(g, (char*)reactionLabel.c_str(), 1);
            agsafeset(reactionNode, (char*)"shape", (char*)"oval", (char*)"");
            agsafeset(reactionNode, (char*)"style", (char*)"filled", (char*)"");
            agsafeset(reactionNode, (char*)"fillcolor", (char*)"yellow", (char*)"");

            for (const auto& input : r.getInputs()) {
                agedge(g, speciesNodes[input], reactionNode, 0, 1);
            }
            for (const auto& output : r.getOutputs()) {
                agedge(g, reactionNode, speciesNodes[output], 0, 1);
            }

            reactionCount++;
        }

        gvLayout(gvc, g, (char*)"dot");
        gvRenderFilename(gvc, g, (char*)"png", filename.c_str());
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
    }

} // namespace sim
