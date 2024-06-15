#include "GraphVisualizer.h"
#include <graphviz/gvc.h>
#include <iostream>

namespace sim {
    GraphVisualizer::GraphVisualizer(const std::vector<Species> &species, const std::vector<sim::Reaction> &reactions)
            : species(species), reactions(reactions) {}

    void GraphVisualizer::generateGraph(const std::string &filename) {
        GVC_t *gvc = gvContext();
        if (!gvc) {
            std::cerr << "Error: Could not create Graphviz context." << std::endl;
            return;
        }

        Agraph_t *g = agopen(const_cast<char *>("G"), Agdirected, nullptr);
        if (!g) {
            std::cerr << "Error: Could not create graph." << std::endl;
            return;
        }

        // Create nodes for each species
        std::map<std::string, Agnode_t *> nodes;
        for (const auto &s: species) {
            if (s.isEnv()) {
                continue; // Skip environment nodes
            }
            nodes[s.getName()] = agnode(g, const_cast<char *>(s.getName().c_str()), 1);
            if (!nodes[s.getName()]) {
                std::cerr << "Error: Could not create node for species " << s.getName() << std::endl;
                continue;
            }
            agsafeset(nodes[s.getName()], const_cast<char *>("label"), const_cast<char *>(s.getName().c_str()),
                      const_cast<char *>(""));
            agsafeset(nodes[s.getName()], const_cast<char *>("shape"), const_cast<char *>("box"),
                      const_cast<char *>(""));
            agsafeset(nodes[s.getName()], const_cast<char *>("style"), const_cast<char *>("filled"),
                      const_cast<char *>(""));
            agsafeset(nodes[s.getName()], const_cast<char *>("fillcolor"), const_cast<char *>("cyan"),
                      const_cast<char *>(""));
        }

        // Create edges for each reaction
        for (const auto &r: reactions) {
            std::string reactionLabel = std::to_string(r.getRate());
            Agnode_t *reactionNode = agnode(g, const_cast<char *>(reactionLabel.c_str()), 1);
            if (!reactionNode) {
                std::cerr << "Error: Could not create reaction node for " << reactionLabel << std::endl;
                continue;
            }
            agsafeset(reactionNode, const_cast<char *>("label"), const_cast<char *>(reactionLabel.c_str()),
                      const_cast<char *>(""));
            agsafeset(reactionNode, const_cast<char *>("shape"), const_cast<char *>("oval"), const_cast<char *>(""));
            agsafeset(reactionNode, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(reactionNode, const_cast<char *>("fillcolor"), const_cast<char *>("yellow"),
                      const_cast<char *>(""));

            for (const auto &input: r.getInputs()) {
                if (nodes.find(input) == nodes.end()) {
                    std::cerr << "Error: Input node " << input << " not found." << std::endl;
                    continue;
                }
                agedge(g, nodes[input], reactionNode, nullptr, 1);
            }

            for (const auto &output: r.getOutputs()) {
                if (output == "env") {
                    continue; // Skip environment nodes
                }
                if (nodes.find(output) == nodes.end()) {
                    std::cerr << "Error: Output node " << output << " not found." << std::endl;
                    continue;
                }
                agedge(g, reactionNode, nodes[output], nullptr, 1);
            }
        }

        if (gvLayout(gvc, g, "dot") != 0) {
            std::cerr << "Error: Could not layout graph." << std::endl;
            agclose(g);
            gvFreeContext(gvc);
            return;
        }
        if (gvRenderFilename(gvc, g, "png", filename.c_str()) != 0) {
            std::cerr << "Error: Could not render graph to file " << filename << std::endl;
        }
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
    }
}