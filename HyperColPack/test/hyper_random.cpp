#include <cstdlib>
#include <set>
#include <fstream>

#include "hyper_graph.hpp"
#include "hyper_symmetric_partition.hpp"

#define N 1000
#define M 400000
#define RANDOM_SEED 12345

using HyperColPack::MultiIndexSet;
using HyperColPack::HyperGraph;
using HyperColPack::HyperSymmetricPartition;


void add_edge(HyperGraph& hg, int v1, int v2, int v3) {
  MultiIndexSet mi;
  mi.insert(v1).insert(v2).insert(v3);
  hg.add_edge(mi);
}
HyperGraph get_random_hyper_graph() {
  HyperGraph hg(N, 3);
  std::set<MultiIndexSet> _edge_set;
  srand(RANDOM_SEED);
  for (int i = 0; i < M; i++) {
    bool new_edge = false;
    //std::cout << " i = " << i << std::endl;
    while (!new_edge) {
      int v1 = rand() % N;
      int v2 = rand() % N;
      int v3 = rand() % N;
      MultiIndexSet e_index_set;
      e_index_set.insert(v1).insert(v2).insert(v3);
      if (_edge_set.find(e_index_set) == _edge_set.end()) {
        new_edge = true;
        //std::cout << "Edge [" << i << "] : ";
        //e_index_set.dump();
        //std::cout << std::endl;
        hg.add_edge(e_index_set);
        _edge_set.insert(e_index_set);
      }
    }
  }
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
  //HyperGraph hg = get_third_diagonal_graph(N);
  HyperGraph hg = get_random_hyper_graph();
  std::ofstream os("test.mm", std::ofstream::out);
  hg.dump_distance_1_graph(os);
  os.close();
  HyperSymmetricPartition sp(hg);
  sp.try_coloring();
  //sp.dump();
  sp.check();
  std::cout << "N = "<< N << ", M = " << M <<", #Colors = " << sp.get_num_of_colors() << std::endl;
}
