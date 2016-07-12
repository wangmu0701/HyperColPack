#ifndef HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_
#define HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_

#include <map>
#include <set>
#include <vector>

#include "hyper_graph.hpp"

#define MAX_ALTERNATIVE_ATTEMPTS 10

namespace HyperColPack {

template <int DEGREE>
class HyperSymmetricPartition {
 public:
  // Constructor
  HyperSymmetricPartition(HyperGraph<DEGREE>& g) {
    v_colors.resize(g._n, -1);
    for (HyperEdge<DEGREE>& edge : g._edge_vec) {
      _unrecovered_edges.insert(&edge);
      _all_edges.insert(&edge);
      for (int i = 0; i < DEGREE; i++) {
        int v = edge.get_v_index(i);
        if (_adjacent_edges[v].find(&edge) == _adjacent_edges[v].end()) {
          _adjacent_edges[v].insert(&edge);
        }
      }
    }
    _colored_v_num = 0;
    _num_of_colors = 0;
  }

  // run the coloring procedure
  void try_coloring() {
    if (_num_of_colors != 0) {
      if (check()) {
        std::cout << "Duplicated call of try_coloring()?" << std::endl;
      } else {
        std::cout << "Check() failed. Something is wrong. :(" << std::endl;
      }
      return;
    }
    int v = -1;
    int* valid_colors = new int[v_colors.size()];
    while ((v = get_next_v()) != -1) {
//    std::cout << "trying to color vertex : " << v
//              << "    current num_of_colors : " << _num_of_colors << std::endl;

      for (int i = 0; i < _num_of_colors; i++) {
        valid_colors[i] = 0;
      }
      for (int c = 0; c < _num_of_colors; c++) {
        //std::cout << "checking color : " << c;
        _alternative_edge_set.clear();
        for (HyperEdge<DEGREE>* edge : _adjacent_edges[v]) {
          //std::cout << "checking edge : ";
          //edge->dump();
          //std::cout << std::endl;
          // first check if color v as c will cause conflict;
          if (!check_conflict(edge, v, c)) {
            valid_colors[c] = -1;
            //std::cout << " invalid" << std::endl;
            break;
          }
          if (_unrecovered_edges.find(edge) != _unrecovered_edges.end()) {
            // edge is still unrecoverable, check if it's recoverable under v#c
            if (!check_recoverable(edge, v, c)) {
              valid_colors[c] = -1;
              //std::cout << " invalid" << std::endl;
              break;
            }
          }
        }
        if (valid_colors[c] != -1) {
          break;
        }
      }
      int c = 0;
      // c will be a valid color or curr #colors if none is valid
      while (c < _num_of_colors && valid_colors[c] == -1) {c++;}
      if (c == _num_of_colors) { _num_of_colors++;}
      // v has color c;
//    std::cout << "vertex " << v << " have color " << c << std::endl;
      v_colors[v] = c;
      // update unrecovered/recovered edges AND compressed/recover index set
      for (HyperEdge<DEGREE>* edge : _adjacent_edges[v]) {
        MultiIndexSet<DEGREE-1> e_index_set = edge->get_v_index_set().remove(v);
        _compressed_index_set[c].insert(e_index_set);
        if (_unrecovered_edges.find(edge) != _unrecovered_edges.end()) {
          _unrecovered_edges.erase(edge);
          _recover_index_set[c].insert(std::make_pair(e_index_set, edge));
          _recovery_index.insert(std::make_pair(edge, v));
        }
      }
    }
    delete[] valid_colors;
  }

  // get the next vertex that we should try to color
  // -1 : we done the coloring!
  int get_next_v() {
    if (_colored_v_num < v_colors.size()) {
      return _colored_v_num++;
    }
    return -1; // done
  }
  
  // 1: e is adjacent to v, 
  // 2: on slice [c], _compressed_index_set[e/v] should be zero
  // 3: do this for ALL UNRECOVERED edges adjacent to v
  bool check_recoverable(HyperEdge<DEGREE>* e, int v, int c) {
    MultiIndexSet<DEGREE-1> e_index_set = e->get_v_index_set().remove(v);
    if (_compressed_index_set[c].find(e_index_set) == _compressed_index_set[c].end()) {
    return true;
    }
    return false;
  }

  // 1: e is adjacent to v
  // 2: on slice [c], _recover_index_set[e/v] should be zero
  // 3: do this for ALL edges adjacent to v
  bool check_conflict(HyperEdge<DEGREE>* e, int v, int c) {
    MultiIndexSet<DEGREE-1> e_index_set = e->get_v_index_set().remove(v);
    typename std::map<MultiIndexSet<DEGREE-1>, HyperEdge<DEGREE>*>::iterator f_iter =
        _recover_index_set[c].find(e_index_set);
    if (f_iter != _recover_index_set[c].end()) {
      return try_another_recover_v(f_iter->second, _recovery_index[f_iter->second]);
//    return false;
    }
    //std::cout << "TRUE" << std::endl;
    return true;
  }
  
  // something will make e UNrecoverable on v
  // can we find another colored vertex that make e recoverable?
  // If yes, use that vertex to recover e, so we can resolve some conflicts
  bool try_another_recover_v(HyperEdge<DEGREE>* e, int v) {
    if (_alternative_edge_set.find(e) != _alternative_edge_set.end()) {
      return false;
    }
    if (_alternative_attempts[e] < MAX_ALTERNATIVE_ATTEMPTS) {

      for (int i = 0; i < DEGREE; i++) {
        int v_alt = e->get_v_index(i);
        if (v_alt != v && v_colors[v_alt] != -1) {
          _alternative_attempts[e]++;
          MultiIndexSet<DEGREE-1> e_index_set = e->get_v_index_set().remove(v_alt);
          // this is the only one, so it can be used in recover
          if (_compressed_index_set[v_colors[v_alt]].count(e_index_set) == 1) {
            _recovery_index[e] = v_alt;
            _recover_index_set[v_colors[v_alt]].insert(
                std::make_pair(e_index_set, e));
            e_index_set = e->get_v_index_set().remove(v);
            _recover_index_set[v_colors[v]].erase(e_index_set); 
            //std::cout << "alter edge : ";
            //e->dump();
            //std::cout << "from " << v << " to " << v_alt << std::endl;
            _alternative_edge_set.insert(e);
            //this->dump();
            return true;
          }
        }
      }
    }
    return false;
  }

  // utilities
  int get_num_of_colors() const {return _num_of_colors;}
  void dump() const {
    std::cout << "Unrecovered edges [" << _unrecovered_edges.size() << "]"<<std::endl;
    for (auto edge : _unrecovered_edges) {
      std::cout << "  ";
      edge->dump();
      std::cout << std::endl;
    }
    std::cout << "Recovered edges [" << _recovery_index.size() << "]" << std::endl;
    for (auto kv : _recovery_index) {
      std::cout << "  ";
      kv.first->dump();
      std::cout << " R{"<< kv.second << "}" << std::endl;
    }
    for (int i = 0; i < v_colors.size(); i++) {
      std::cout << "color["<<i<<"] = " << v_colors[i] << std::endl;
    }
    std::cout << "On Each Recovery Slice" << std::endl;
    for (auto kv : _recover_index_set) {
      std::cout << "On slice " << kv.first << "  :  "<< std::endl;
      for (auto kv2 : kv.second) {
        std::cout << "    ";
        kv2.first.dump();
        std::cout << std::endl;
      }
    }
    std::cout << "On Each Compressed Slice" << std::endl;
    for (auto kv : _compressed_index_set) {
      std::cout << "On slice " << kv.first << " : " << std::endl;
      for (const MultiIndexSet<DEGREE-1>& index_set : kv.second) {
        std::cout << "    ";
        index_set.dump();
        std::cout << std::endl;
      }
    }
  }

  bool check() const {
    std::map<int, std::map<MultiIndexSet<DEGREE-1>, int>> _check_count;
    if (_unrecovered_edges.size() != 0) {
      std::cout << "Check Fail!!! Still unrecovered edges remains!" << std::endl;
      return false;
    }
    for (HyperEdge<DEGREE>* edge : _all_edges) {
      int prev_v = -1;
      for (int i = 0; i < DEGREE; i++) {
        int v = edge->get_v_index(i);
        if (v != prev_v) {
          MultiIndexSet<DEGREE-1> edge_index_set =
              edge->get_v_index_set().remove(v);
          _check_count[v_colors[v]][edge_index_set]++;
          //edge_index_set.dump();
          //std::cout << std::endl;
          prev_v = v;
        }
      }
    }
    for (HyperEdge<DEGREE>* edge : _all_edges) {
      int r = _recovery_index.at(edge);
      MultiIndexSet<DEGREE-1> r_index_set =
          edge->get_v_index_set().remove(r);
      if (_check_count[v_colors[r]][r_index_set] != 1) {
        std::cout << "Check Fail!!! Unrecoverable for edge :";
        edge->dump();
        std::cout << " on R{" << r << "}" << std::endl;
        std::cout << "color = " << v_colors[r] << " R index = ";
        r_index_set.dump();
        std::cout << std::endl;
        std::cout << "count = " << _check_count[v_colors[r]][r_index_set] << std::endl;
        return false;
      }
    }
    std::cout << "Check success!"<< std::endl;
    return true;
  }

 private:
  // number of colored vertices
  int _colored_v_num;

  int _num_of_colors;

  // map v -> adjacent edges
  std::map<int, std::set<HyperEdge<DEGREE>*>> _adjacent_edges;

  // all unrecovered_edges;
  std::set<HyperEdge<DEGREE>*> _unrecovered_edges;
  std::set<HyperEdge<DEGREE>*> _all_edges;
  
  // color[v];
  std::vector<int> v_colors;

  // number of attempts that this edge have tried for an alternative recover
  std::map<HyperEdge<DEGREE>*, int> _alternative_attempts;

  // this is tricky, otherwise there will be a bug
  // For example, an edge {1,2,3} is initially recovered by 1
  // The current v, when trying color #1, will alter it to index 3.
  // then when trying color #2, will alter it back to index 1.
  // Which makes us think color #1 is valid, but under current setting it's not.
  std::set<HyperEdge<DEGREE>*> _alternative_edge_set;

  // map HyperEdge -> recovery index
  std::map<HyperEdge<DEGREE>*, int> _recovery_index;

  // the non-zero index set on each slice
  // a new recovery index can NOT overlap with any of these
  std::map<int, std::multiset<MultiIndexSet<DEGREE-1>>> _compressed_index_set;

  // the recovery index set on each slice
  // a new non-recovery index should NOT overlap with any of these
  std::map<int, std::map<MultiIndexSet<DEGREE-1>, HyperEdge<DEGREE>*>> _recover_index_set;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_
