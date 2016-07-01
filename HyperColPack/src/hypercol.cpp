#include <iostream>
#include "hyper_graph.hpp"
#include "hyper_edge.hpp"

namespace HyperColPack {
/*
void SymmetricPartition(HyperGraph& G) {
  std::cout << "in Symmetric Partition" << std::endl;

  // get a specific order for vertices to process
  // default is 0, 1, 2, ..., n-1
  std::vector<int> vertex_vec = G.get_vertices_vec();
  int curr_color = 0;
  bool* valid_colors = new bool[vertex_vec.size()];
  int n = vertex_vec.size();
  for (int vi = 0; vi < vertex_vec.size(); vi++) {
    int v = vertex_vec[vi];
    // now process v
    // loop over all unrecovered edge adjacent to v
    for (int i = 0; i < curr_color; i++) {
      valid_colors[i] = true;
    }
    std::vector<HyperEdge*> edge_vec = G.get_unrecovered_edges(v);
    std::vector<HyperEdge*> all_edge_vec = G.get_all_edges(v);
    //std::cout << "Vertex = " << v << std::endl;
    for (int color = 0; color < curr_color; color++) {
      bool still_valid = true;
      // current recover-index can not be overwritten
      for (int ei = 0; ei < all_edge_vec.size() && still_valid; ei++) {
        HyperEdge* ep = all_edge_vec[ei];
        if (!G.check_conflict(ep, v, color)) {
          //std::cout << "Color " << color << " have conflict on Edge ";
          //ep->dump();
          valid_colors[color] = false;
          still_valid = false;
        }
      }
      // recoverable edge can not be conflict with existing multi-index
      for (int ei = 0; ei < edge_vec.size() && still_valid; ei++) {
        HyperEdge* ep = edge_vec[ei];
        if (!G.check_recoverable(ep, v, color)) {
          //std::cout << "Color " << color << " non-recoverable on Edge ";
          //ep->dump();
          valid_colors[color] = false;
          still_valid = false;
        }
      }
    }
    int color = 0;
    while (color < curr_color && !valid_colors[color]) {
      //std::cout << "Vertex " << v << " invalid on color " << color << std::endl;
      color++;
    }
    if (color == curr_color) {
      curr_color++;
    }
    G.set_color(v, color);
    //G.dump();
  }
 
}
*/
} // HyperColPack
