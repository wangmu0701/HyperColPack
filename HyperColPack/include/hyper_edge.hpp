#ifndef HYPERCOLPACK_HYPER_EDGE_H_
#define HYPERCOLPACK_HYPER_EDGE_H_

#include "multi_index_set.hpp"

namespace HyperColPack {

template <int DEGREE> class HyperGraph;

template <int DEGREE>
class HyperEdge {
 public:
  // The default constructor is deleted, so the ONLY way you can construct
  // a HyperEdge is via HyperGraph.add_edge();
  HyperEdge() = delete;

  bool operator < (const HyperEdge& rhs) const {
    return this->_v_index_set < rhs._v_index_set;
  }
/*
  MultiIndexSet get_quotient_index() const {
    MultiIndexSet ret = _adjacent_index;
    if (curr_recovery_index != -1) {
      ret.remove(curr_recovery_index);
    }
    return ret;
  }

  MultiIndexSet get_quotient_index(int v) const {
    MultiIndexSet ret = _adjacent_index;
    ret.remove(v);
    return ret;
  }
*/
  MultiIndexSet<DEGREE> get_v_index_set() {
    return _v_index_set;
  }

  int get_v_index(int index) {
    return _v_index_set.get(index);
  } 
/* 
  void set_recover_index(int v) {
    curr_recovery_index = v;
    curr_recovery_location = _adjacent_index.find(v);
    checked[curr_recovery_location] = true;
  }

  int get_recover_index() const {
    return curr_recovery_index;
  }

  int get_recover_location() const {
    return curr_recovery_location;
  }
*/
  bool adjacent_to(int v) const {
    return _v_index_set.contains(v);
  }
/*
  int get_edge_id() const {
    return _edge_id;
  }
  int get_vertex(int index) const {
    return _adjacent_index.get(index);
  }
  bool not_checked(int index) const {
    return !checked[index];
  }
  void set_checked(int index) {
    checked[index] = true;
  }
*/
  void dump() const {
    std::cout << "Edge["<<_edge_id<<"] : ";
    _v_index_set.dump();
    //std::cout << std::endl;
  }
  

 private:
  // Private constructor that will be called by HyperGraph.add_edge();
  HyperEdge(int id, const MultiIndexSet<DEGREE>& multi_index) {
    _edge_id = id;
    _v_index_set = multi_index;
  }

  // adjacent vertices
  int _edge_id;
  MultiIndexSet<DEGREE> _v_index_set;
  
  friend class HyperGraph<DEGREE>;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_EDGE_H_
