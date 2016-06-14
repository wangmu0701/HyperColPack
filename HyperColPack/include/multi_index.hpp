#ifndef HYPERCOLPACK_MULTI_INDEX_H_
#define HYPERCOLPACK_MULTI_INDEX_H_

#include <iostream>

namespace HyperColPack {

class MultiIndex {
 public:
  MultiIndex() : _len(0) {}
  MultiIndex(const MultiIndex&) = default;
  MultiIndex& operator = (const MultiIndex&) = default;

  bool operator < (const MultiIndex& rhs) const {
    if (this->_len < rhs._len) {
      return true;
    } else if (this->_len > rhs._len) {
      return false;
    }
    for (int i = 0; i < this->_len; i++) {
      if (this->_data[i] < rhs._data[i]) {
        return true;
      } else if (this->_data[i] > rhs._data[i]) {
        return false;
      }
    }
    return false;
  }

  MultiIndex& insert(int x) {
    int l = 0;
    while ((l < _len) && (_data[l] < x)) {
      l++;
    }
    for (int i = _len; i >= l+1; i--) {
      _data[i] = _data[i-1];
    }
    _data[l] = x;
    _len += 1;
    return (*this);
  }

  MultiIndex& remove(int x) {
    int l = 0;
    while (l < _len && _data[l] != x) {
      l++;
    }
    if (l == _len) {return *this;}
    for (int i = l; i < _len - 1; i++) {
      _data[i] = _data[i+1];
    }
    _len -= 1;
    return *this;
  }

  MultiIndex& remove_location(int l) {
    for (int i = l; i < _len - 1; i++) {
      _data[i] = _data[i+1];
    }
    _len -= 1;
    return *this;
  } 
  bool contains(int v) const {
    for (int i = 0; i < _len; i++) {
      if (_data[i] == v) {
        return true;
      } else if (_data[i] > v) {
        return false;
      }
    }
    return false;
  }
  int find(int v) const {
    for (int i = 0; i < _len; i++) {
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
  void clear() {_len = 0;}
  int size() const {return _len;}
  int* get_array() {return &(_data[0]);}

  void dump() const {
    dump(true);
  }
  void dump(bool endline) const {
    std::cout << "Multi-index : { ";
    for (int i = 0; i < _len; i++) {
      std::cout << _data[i] << " ";
    }
    std::cout << "}";
    if (endline) {
      std::cout << std::endl;
    }
  }

 private:
  int _data[10];
  int _len;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_MULTI_INDEX_H_
