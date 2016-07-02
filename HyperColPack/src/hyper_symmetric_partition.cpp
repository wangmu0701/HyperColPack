#include "hyper_symmetric_partition.hpp"

namespace HyperColPack {

HyperSymmetricPartition::HyperSymmetricPartition(const HyperGraph& g) {
  v_colors.resize(g._n, -1);
  for (std::shared_ptr<HyperEdge> edge : g._edge_vec) {
    _unrecovered_edges.insert(edge);
    _all_edges.insert(edge);
    for (int i = 0; i < edge->get_degree(); i++) {
      int v = edge->get_v_index(i);
      if (_adjacent_edges[v].find(edge) == _adjacent_edges[v].end()) {
        _adjacent_edges[v].insert(edge);
      }
    }
  }
  _colored_v_num = 0;
}

void HyperSymmetricPartition::try_coloring() {
  std::cout << "I'm trying to color : " << v_colors.size() << std::endl;
  int* valid_colors = new int[v_colors.size()];
  int curr_colors = 0;
  int v = -1;
  while ((v = get_next_v()) != -1) {
    std::cout << "trying to color vertex : " << v
              << "    curr_colors : " << curr_colors << std::endl;
    for (int i = 0; i < curr_colors; i++) {
      valid_colors[i] = 0;
    }
    for (std::shared_ptr<HyperEdge> edge : _adjacent_edges[v]) {
      //std::cout << "checking edge : ";
      //edge->dump();
      //std::cout << std::endl;
      for (int c = 0; c < curr_colors; c++) {
        //std::cout << "checking color : " << c;
        if (valid_colors[c] == 0) {
          // first check if color v as c will cause conflict;
          if (!check_conflict(edge, v, c)) {
            valid_colors[c] = -1;
            //std::cout << " invalid" << std::endl;
            continue;
          }
          if (_unrecovered_edges.find(edge) != _unrecovered_edges.end()) {
            // edge is still unrecoverable, check if it's recoverable under v#c
            if (!check_recoverable(edge, v, c)) {
              valid_colors[c] = -1;
              //std::cout << " invalid" << std::endl;
            }
          }
        }
      }
    }
    int c = 0;
    // c will be a valid color or curr_colors if none is valid
    while (c < curr_colors && valid_colors[c] == -1) {c++;}
    if (c == curr_colors) { curr_colors++;}
    // v has color c;
    std::cout << "vertex " << v << " have color " << c << std::endl;
    v_colors[v] = c;
    // update unrecovered/recovered edges AND compressed/recover index set
    for (std::shared_ptr<HyperEdge> edge : _adjacent_edges[v]) {
      MultiIndexSet e_index_set = edge->get_v_index_set();
      e_index_set.remove(v);
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

int HyperSymmetricPartition::get_next_v() {
  if (_colored_v_num < v_colors.size()) {
    return _colored_v_num++;
  }
  return -1; // done
}

bool HyperSymmetricPartition::check_recoverable(
    std::shared_ptr<HyperEdge> e, int v, int c) {
  MultiIndexSet e_index_set = e->get_v_index_set();
  e_index_set.remove(v);
  if (_compressed_index_set[c].find(e_index_set) == _compressed_index_set[c].end()) {
    return true;
  }
  return false;
}

bool HyperSymmetricPartition::check_conflict(
    std::shared_ptr<HyperEdge> e, int v, int c) {
  MultiIndexSet e_index_set = e->get_v_index_set();
  e_index_set.remove(v);
  std::map<MultiIndexSet, std::shared_ptr<HyperEdge>>::iterator f_iter = 
      _recover_index_set[c].find(e_index_set);
  if (f_iter != _recover_index_set[c].end()) {
    return try_another_recover_v(f_iter->second,
                                 _recovery_index[f_iter->second]);
  }
  //std::cout << "TRUE" << std::endl;
  return true;
}

bool HyperSymmetricPartition::try_another_recover_v(
    std::shared_ptr<HyperEdge> e, int v) {
  if (_alternative_attempts[e] < MAX_ALTERNATIVE_ATTEMPTS) {
    MultiIndexSet e_index_set = e->get_v_index_set();
    for (int i = 0; i < e->get_degree(); i++) {
      int v_alt = e_index_set.get(i);
      if (v_alt != v && v_colors[v_alt] != -1) {
        _alternative_attempts[e]++;
        e_index_set.remove(v_alt);
        // this is the only one, so it can be used in recover
        if (_compressed_index_set[v_colors[v_alt]].count(e_index_set) == 1) {
          _recovery_index[e] = v_alt;
          _recover_index_set[v_colors[v_alt]].insert(
              std::make_pair(e_index_set, e));
          e_index_set.insert(v_alt).remove(v);
          _recover_index_set[v_colors[v]].erase(e_index_set); 
          //std::cout << "alter edge : ";
          //e->dump();
          //std::cout << "from " << v << " to " << v_alt << std::endl;
          //this->dump();
          return true;
        }
      }
    }
  }
  return false;
}

void HyperSymmetricPartition::dump() const {
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
    for (const MultiIndexSet& index_set : kv.second) {
      std::cout << "    ";
      index_set.dump();
      std::cout << std::endl;
    }
  }
}

bool HyperSymmetricPartition::check() const {
  std::map<int, std::map<MultiIndexSet, int>> _check_count;
  if (_unrecovered_edges.size() != 0) {
    std::cout << "Check Fail!!! Still unrecovered edges remains!" << std::endl;
    return false;
  }
  for (std::shared_ptr<HyperEdge> edge : _all_edges) {
    int d = edge->get_degree();
    MultiIndexSet edge_index_set = edge->get_v_index_set();
    int prev_v = -1;
    for (int i = 0; i < d; i++) {
      int v = edge->get_v_index(i);
      if (v != prev_v) {
        edge_index_set.remove(v);
        _check_count[v_colors[v]][edge_index_set]++;
        //edge_index_set.dump();
        //std::cout << std::endl;
        edge_index_set.insert(v);
        prev_v = v;
      }
    }
  }
  for (std::shared_ptr<HyperEdge> edge : _all_edges) {
    int r = _recovery_index.at(edge);
    MultiIndexSet r_index_set = edge->get_v_index_set();
    r_index_set.remove(r);
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

} // namespace HyperColPack
