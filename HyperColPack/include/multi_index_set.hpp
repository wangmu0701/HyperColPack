#ifndef HYPERCOLPACK_MULTI_INDEX_SET_H_
#define HYPERCOLPACK_MULTI_INDEX_SET_H_

#include <iostream>
#include <initializer_list>

namespace HyperColPack {

class MultiIndexSet {
 public:
  MultiIndexSet() : _size(0) {}
  MultiIndexSet(const MultiIndexSet&) = default;
  MultiIndexSet& operator = (const MultiIndexSet&) = default;

  MultiIndexSet(std::initializer_list<int> v_list) {
    _size = v_list.size();
    int l = 0;
    for (int v_index : v_list) {
      _data[l++] = v_index;
    }
  }
  bool operator < (const MultiIndexSet& rhs) const {
    if (this->_size < rhs._size) {
      return true;
    } else if (this->_size > rhs._size) {
      return false;
    }
    for (int i = 0; i < this->_size; i++) {
      if (this->_data[i] < rhs._data[i]) {
        return true;
      } else if (this->_data[i] > rhs._data[i]) {
        return false;
      }
    }
    return false;
  }

  MultiIndexSet& insert(int x) {
    int l = 0;
    while ((l < _size) && (_data[l] < x)) {
      l++;
    }
    for (int i = _size; i >= l+1; i--) {
      _data[i] = _data[i-1];
    }
    _data[l] = x;
    _size += 1;
    return (*this);
  }

  MultiIndexSet& remove(int x) {
    int l = 0;
    while (l < _size && _data[l] != x) {
      l++;
    }
    if (l == _size) {return *this;}
    for (int i = l; i < _size - 1; i++) {
      _data[i] = _data[i+1];
    }
    _size -= 1;
    return *this;
  }

  MultiIndexSet& remove_location(int l) {
    for (int i = l; i < _size - 1; i++) {
      _data[i] = _data[i+1];
    }
    _size -= 1;
    return *this;
  } 
  bool contains(int v) const {
    for (int i = 0; i < _size; i++) {
      if (_data[i] == v) {
        return true;
      } else if (_data[i] > v) {
        return false;
      }
    }
    return false;
  }
  int find(int v) const {
    for (int i = 0; i < _size; i++) {
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
  void clear() {_size = 0;}
  int size() const {return _size;}
  int* get_array() {return &(_data[0]);}

  void dump() const {
    dump(false);
  }
  void dump(bool endline) const {
    std::cout << "Multi-index : { ";
    for (int i = 0; i < _size; i++) {
      std::cout << _data[i] << " ";
    }
    std::cout << "}";
    if (endline) {
      std::cout << std::endl;
    }
  }

 private:
  int _data[10];
  int _size;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_MULTI_INDEX_SET_H_
