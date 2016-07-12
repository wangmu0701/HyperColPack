#ifndef HYPERCOLPACK_MULTI_INDEX_SET_H_
#define HYPERCOLPACK_MULTI_INDEX_SET_H_

#include <cassert>
#include <iostream>
#include <initializer_list>
#include <vector>

namespace HyperColPack {

template <int DEGREE>
class MultiIndexSet {
 public:
  MultiIndexSet() = default;
  MultiIndexSet(const MultiIndexSet&) = default;
  MultiIndexSet& operator = (const MultiIndexSet&) = default;


  MultiIndexSet(const std::initializer_list<int>& v_list) {
    assert(DEGREE == v_list.size());
    int l = 0;
    for (const int v_index : v_list) {
      _data[l++] = v_index;
    }
  }

  MultiIndexSet(const std::vector<int>& v_list) {
    assert(DEGREE == v_list.size());
    int l = 0;
    for (const int v_index : v_list) {
      _data[l++] = v_index;
    }
  }

  bool operator < (const MultiIndexSet& rhs) const {
    for (int i = 0; i < DEGREE; i++) {
      if (this->_data[i] < rhs._data[i]) {
        return true;
      } else if (this->_data[i] > rhs._data[i]) {
        return false;
      }
    }
    return false;
  }

  MultiIndexSet<DEGREE+1> insert(int x) {
    std::vector<int> v_vec;
    int l = 0;
    while ((l < DEGREE) && (_data[l] < x)) {
      v_vec.push_back(_data[l]);
      l++;
    }
    v_vec.push_back(x);
    for (;l < DEGREE; l++) {
      v_vec.push_back(_data[l]);
    }
    return MultiIndexSet<DEGREE+1>(v_vec);
  }

  MultiIndexSet<DEGREE-1> remove(int x) {
    std::vector<int> v_vec;
    int l = 0;
    while (l < DEGREE && _data[l] != x) {
      v_vec.push_back(_data[l]);
      l++;
    }
    // skip l, because _data[l] == x
    for (l++; l < DEGREE; l++) {
      v_vec.push_back(_data[l]);
    }
    return MultiIndexSet<DEGREE-1>(v_vec);
  }

/*
  MultiIndexSet& remove_location(int l) {
    for (int i = l; i < DEGREE - 1; i++) {
      _data[i] = _data[i+1];
    }
    DEGREE -= 1;
    return *this;
  } 
*/

  bool contains(int v) const {
    for (int i = 0; i < DEGREE; i++) {
      if (_data[i] == v) {
        return true;
      } else if (_data[i] > v) {
        return false;
      }
    }
    return false;
  }

  int find(int v) const {
    for (int i = 0; i < DEGREE; i++) {
      if (_data[i] == v) {
        return i;
      } else if (_data[i] > v) {
        return -1;
      }
    }
    return -1;
  }

  int get(int index) const {
    return _data[index];
  }
  void clear() {DEGREE = 0;}
  int size() const {return DEGREE;}
  int* get_array() {return &(_data[0]);}

  void dump() const {
    dump(false);
  }
  void dump(bool endline) const {
    std::cout << "Multi-index : { ";
    for (int i = 0; i < DEGREE; i++) {
      std::cout << _data[i] << " ";
    }
    std::cout << "}";
    if (endline) {
      std::cout << std::endl;
    }
  }

 private:
  int _data[DEGREE];
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_MULTI_INDEX_SET_H_
