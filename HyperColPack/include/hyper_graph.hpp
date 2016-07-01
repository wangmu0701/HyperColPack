#ifndef HYPERCOLPACK_HYPER_GRAPH_H_
#define HYPERCOLPACK_HYPER_GRAPH_H_

#include <memory>
#include <vector>

#include "hyper_edge.hpp"

namespace HyperColPack {

class HyperSymmetricPartition;

class HyperGraph {
 public:
  HyperGraph() : _n(0), _d(0), _m(0){}
  HyperGraph(int n , int d) : _n(n), _d(d), _m(0){}
  std::shared_ptr<HyperEdge> add_edge(const MultiIndexSet& v_index_set) {
    std::shared_ptr<HyperEdge> edge(new HyperEdge(_m++, v_index_set));
    _edge_vec.push_back(edge);
    return edge;
  } 

  void dump() const {
    std::cout << "HyperGraph v["<<_n<<"], e["<<_m << "]" << std::endl;
    for (const std::shared_ptr<HyperEdge>& e : _edge_vec) {
      std::cout << "  ";
      e->dump();
      std::cout << std::endl;
    }
  }
 private:
  int _n;
  int _m;
  int _d;
  std::vector<std::shared_ptr<HyperEdge>> _edge_vec;

  friend class HyperSymmetricPartition;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_GRAPH_H_
