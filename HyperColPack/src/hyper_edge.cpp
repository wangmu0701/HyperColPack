#include "hyper_edge.hpp"

HyperEdge::HyperEdge(int d, int* v) {
  _d = d;
  _v = new int[d];
  curr_recovery_index = -1;
  _checked = new bool[d];
  for (int i = 0; i < _d; i++) {
    _v[i] = v[i];
    _checked[i] = false;
  }
  // sort _v, later
}

HyperEdge::~HyperEdge() {
  delete[] _v;
  delete[] _checked;
}
