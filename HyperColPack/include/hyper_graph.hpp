#ifndef HYPERCOLPACK_HYPER_GRAPH_H_
#define HYPERCOLPACK_HYPER_GRAPH_H_

#include <algorithm>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <iostream>

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
  void dump_distance_1_graph(std::ostream& os) const {
    std::map<MultiIndexSet, std::set<int>> _collapse_index;
    std::set<std::pair<int, int>> _distance_1_adjacent_set;
    std::vector<std::pair<int, int>> _distance_1_adjacent_vector;
    for (std::shared_ptr<HyperEdge> edge : _edge_vec) {
      MultiIndexSet e_index_set = edge->get_v_index_set();
      int d = edge->get_degree();
      for (int i = 0; i < d; i++) {
        int v = edge->get_v_index(i);
        e_index_set.remove(v);
        _collapse_index[e_index_set].insert(v);
        e_index_set.insert(v);
      }
    }
    for (auto kv : _collapse_index) {
      for (int v1 : kv.second) {
        for (int v2 : kv.second) {
          if (v1 >= v2) {
            _distance_1_adjacent_set.insert(std::make_pair(v1, v2));
          }
        }
      }
    }
    for (auto v_pair : _distance_1_adjacent_set) {
      _distance_1_adjacent_vector.push_back(v_pair);
    }
    std::sort(_distance_1_adjacent_vector.begin(), _distance_1_adjacent_vector.end());
    // MM header
    os << "%%MatrixMarket matrix coordinate pattern symmetric" << std::endl;
    os << _n << " " << _n << " " << _distance_1_adjacent_vector.size() << std::endl;
    for (auto v_pair : _distance_1_adjacent_vector) {
      os << v_pair.first << " " << v_pair.second << std::endl; 
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
