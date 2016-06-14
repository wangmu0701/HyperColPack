#ifndef HYPERCOLPACK_HYPER_EDGE_H_
#define HYPERCOLPACK_HYPER_EDGE_H_

#include "multi_index.hpp"

namespace HyperColPack {

class HyperEdge {
 public:
  HyperEdge(int id, const MultiIndex& multi_index) {
    _edge_id = id;
    _adjacent_index = multi_index;
    curr_recovery_index = -1;
    curr_recovery_location = -1;
    for (int i = 0; i < multi_index.size(); i++) {
      checked[i] = false;
    }
  }

  bool operator < (const HyperEdge& rhs) const {
    return this->_adjacent_index < rhs._adjacent_index;
  }

  MultiIndex get_quotient_index() const {
    MultiIndex ret = _adjacent_index;
    if (curr_recovery_index != -1) {
      ret.remove(curr_recovery_index);
    }
    return ret;
  }

  MultiIndex get_quotient_index(int v) const {
    MultiIndex ret = _adjacent_index;
    ret.remove(v);
    return ret;
  }

  MultiIndex get_index() {
    return _adjacent_index;
  }
  
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

  bool adjacent_to(int index) const {
    return _adjacent_index.contains(index);
  }

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
  void dump() const {
    std::cout << "Edge["<<_edge_id<<"] : <" << curr_recovery_index << "> ";
    _adjacent_index.dump();
  }
  

 private:
  // degree and adjacent vertices
  int _edge_id;
  MultiIndex _adjacent_index;

  int curr_recovery_index;
  int curr_recovery_location;
  bool checked[10];
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_EDGE_H_
