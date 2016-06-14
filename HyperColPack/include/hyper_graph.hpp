#ifndef HYPERCOLPACK_HYPER_GRAPH_H_
#define HYPERCOLPACK_HYPER_GRAPH_H_

#include <set>
#include <vector>
#include <map>

#include "hyper_edge.hpp"

namespace HyperColPack {

class HyperGraph {
 public:
  HyperGraph() = default;
  // a new graph with n nodes, edge degree d,
  HyperGraph(int n, int d) : _n(n), _d(d), _m(0) {
    _v_colors = new int[_n];
    for (int i = 0; i < _n; i++) {
      _v_colors[i] = -1;
    }
  };
  ~HyperGraph() {
    delete[] _v_colors;
  }

  void add_edge(const MultiIndex& multi_index) {
    HyperEdge* edge = new HyperEdge(_m, multi_index);
    _edge_vec.push_back(edge);
    _unrecovered_edge.insert(edge);
    _m++;
  }

  std::vector<int> get_vertices_vec() {
    std::vector<int> ret;
    for (int i = 0; i < _n; i++) {
      ret.push_back(i);
    }
    return ret;
  }

  std::vector<HyperEdge*> get_unrecovered_edges(int v) {
    std::vector<HyperEdge*> ret;
    for (HyperEdge* edge : _unrecovered_edge) {
      if (edge->adjacent_to(v)) {
        ret.push_back(edge);
      }
    }
    return ret;
  }

  std::vector<HyperEdge*> get_all_edges(int v) {
    std::vector<HyperEdge*> ret;
    for (HyperEdge* edge : _edge_vec) {
      if (edge->adjacent_to(v)) {
        ret.push_back(edge);
      }
    }
    return ret;
  } 

  bool check_recoverable(HyperEdge* edge, int vertex, int color) {
    MultiIndex index = edge->get_index();
    index.remove(vertex);
    //std::cout << "Checking : "; index.dump(false);
    //std::cout << " on color = " << color << std::endl;
    if (_conflict_index[color].find(index) != _conflict_index[color].end()) {
      return false;
    }
    return true; 
  }

  bool check_conflict(HyperEdge* edge, int vertex, int color) {
    MultiIndex index = edge->get_index();
    index.remove(vertex);

    if (_recover_mapping[color].find(index) != _recover_mapping[color].end()) {
      // before mark it invalid, give it another try
//    std::cout << "Checking : "; index.dump(false);
//    std::cout << "for v = " << vertex << " on color = " << color << std::endl;
//    this->dump();
      HyperEdge* ep = _recover_mapping[color].find(index)->second;
//std::cout << "trying reassign edge "; ep->dump();
      int l = ep->get_recover_location();
      int v = ep->get_recover_index();
      for (int i = l+1; i < _d; i++) {
        if (ep->not_checked(i)) {
          int tv = ep->get_vertex(i);
//std::cout << "checking vertex = " << tv << std::endl;
//std::cout << "v = " << v << std::endl;
//std::cout << "tc = " << _v_colors[tv] << std::endl;
          if (tv == v) {
            ep->set_checked(i);
          } else if (_v_colors[tv] != -1) {
            int tc = _v_colors[tv];
            MultiIndex qi = ep->get_index();
            qi.remove(tv);
//std::cout << "tc = " << tc << std::endl;
//std::cout << "qi = "; qi.dump();
            if (_conflict_index[tc][qi] == 1) {
              // ep is recoverable in tc
//std::cout << "Edge : ";
//ep->get_index().dump(false);
//std::cout << "Moved recovery from " << v << " to " << tv << std::endl;
              ep->set_recover_index(tv);
              _recover_mapping[color].erase(index);
              _recover_mapping[tc].insert(
                  std::pair<MultiIndex, HyperEdge*>(qi, ep));
              return true;
            }
          }
        }
      }

      return false;
    }
    return true; 
  }
 
  void set_color(int v, int color) {
    std::vector<HyperEdge*> edge_vec = get_unrecovered_edges(v);
    std::vector<HyperEdge*> all_edge_vec = get_all_edges(v);
    for (HyperEdge* edge : edge_vec) {
      _unrecovered_edge.erase(edge);
      _recovered_edge.erase(edge);
      edge->set_recover_index(v);
      _recover_mapping[color].insert(
          std::pair<MultiIndex, HyperEdge*>(edge->get_quotient_index(), edge));
    }
    for (HyperEdge* edge : all_edge_vec) {
      //edge->dump();
      MultiIndex mi = edge->get_quotient_index(v);
      //std::cout << "Put "; mi.dump(false);
      //std::cout << "in color " << color << std::endl;
      _conflict_index[color][edge->get_quotient_index(v)]++;
    }
    _colors[color].insert(v);
    _v_colors[v] = color;
  }

  void recovery_directional_hessian(
      int color, int size, int** tind, double* values,
      std::map<int, std::map<int, std::map<int, double> > >& third) {
    for (int i = 0; i < size; i++) {
      MultiIndex mi;
      mi.insert(tind[i][0]).insert(tind[i][1]);
      if (_recover_mapping[color].find(mi) != _recover_mapping[color].end()) {
        mi.insert(_recover_mapping[color].find(mi)->second->get_recover_index());
        int* t = mi.get_array();
        int v1 = t[0];
        int v2 = t[1];
        int v3 = t[2];
        third[v1][v2][v3] = values[i];
      }
    }
  } 

  int get_num_color() const {
    return _colors.size();
  }
  int get_num_vertices() const {
    return _n;
  }
  int get_direction(int color, double* d) {
    for (int i = 0; i < _n; i++) {
      d[i] = 0;
    }
    for (int v : _colors[color]) {
      d[v] = 1.0;
    }
  }

  void dump() const {
    std::cout << "Graph[" << _n << ", " << _d << "] : " << std::endl;
    for(const HyperEdge* edge : _edge_vec) {
      edge->dump();
    }
    std::cout << "number of colors = " << _colors.size() << std::endl;
    for (const auto& kv : _colors) {
      std::cout << "Color " << kv.first << " : { ";
      for (int v : kv.second) {
        std::cout << v << " "; 
      }
      std::cout << "}" << std::endl;
      for (const auto& kv2 : _recover_mapping.find(kv.first)->second) {
        kv2.first.dump(false);
        std::cout << " -----> " << kv2.second->get_recover_index() << " Edge[" << kv2.second->get_edge_id() << "]" << std::endl; 
      }
      std::cout << "  Conflict Index Set : {" << std::endl;
      for (const auto& kv2 : _conflict_index.find(kv.first)->second) {
        kv2.first.dump(false);
        std::cout << " Counts ["<< kv2.second << "]" << std::endl; 
      }
      std::cout << "                       }" << std::endl;
    }
    
  }
 private:
  int _n;
  int _d;
  int _m;
  int* _v_colors;
  std::vector<HyperEdge*> _edge_vec;
  std::set<HyperEdge*> _recovered_edge;
  std::set<HyperEdge*> _unrecovered_edge;
  std::map<int, std::set<int> > _colors;
  std::map<int, std::map<MultiIndex, HyperEdge*> > _recover_mapping;
  std::map<int, std::map<MultiIndex, int> > _conflict_index;  
};


} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_GRAPH_H_
