#include <fstream>
#include "hyper_graph.hpp"
#include "hyper_symmetric_partition.hpp"

using HyperColPack::MultiIndexSet;
using HyperColPack::HyperGraph;
using HyperColPack::HyperSymmetricPartition;

void add_edge(HyperGraph& hg, int v1, int v2) {
  MultiIndexSet mi;
  mi.insert(v1).insert(v2);
  hg.add_edge(mi);
}
void add_edge(HyperGraph& hg, int v1, int v2, int v3) {
  MultiIndexSet mi;
  mi.insert(v1).insert(v2).insert(v3);
  hg.add_edge(mi);
}
HyperGraph get_hessian_graph() {
  HyperGraph hg(10, 2);
  add_edge(hg, 0, 0);
  add_edge(hg, 0, 1);
  add_edge(hg, 0, 6);
  add_edge(hg, 1, 1);
  add_edge(hg, 1, 2);
  add_edge(hg, 1, 4);
  add_edge(hg, 2, 2);
  add_edge(hg, 2, 3);
  add_edge(hg, 2, 5);
  add_edge(hg, 3, 3);
  add_edge(hg, 3, 9);
  add_edge(hg, 4, 4);
  add_edge(hg, 4, 5);
  add_edge(hg, 4, 7);
  add_edge(hg, 5, 5);
  add_edge(hg, 5, 8);
  add_edge(hg, 6, 6);
  add_edge(hg, 6, 7);
  add_edge(hg, 7, 7);
  add_edge(hg, 7, 8);
  add_edge(hg, 8, 8);
  add_edge(hg, 8, 9);
  add_edge(hg, 9, 9);
  return hg;
}

HyperGraph get_third_diagonal_graph(int n) {
  HyperGraph hg(n, 3);
  for (int i = 0; i < n; i++) {
    add_edge(hg, i, i, i);
    if (i != n-1) {
      add_edge(hg, i, i, i+1);
      add_edge(hg, i, i+1, i+1);
    }
    if (i != n-1 && i != 0) {
      add_edge(hg, i-1, i, i+1);
    }
  }
  return hg;
}
int main() {
  //HyperGraph hg = get_hessian_graph();
  HyperGraph hg = get_third_diagonal_graph(10);
  HyperSymmetricPartition sp(hg);
  sp.try_coloring();
  sp.dump();
  sp.check();
  //hg.dump_distance_1(std::ofstream("text.out", std::ofstream::out));
  //hg.dump_distance_1(std::cout);
  std::ofstream os("test.mm", std::ofstream::out);
  hg.dump_distance_1_graph(os);
  os.close();
}
