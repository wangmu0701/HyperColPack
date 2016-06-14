#include <map>
#include <set>

#include "multi_index.hpp"

using HyperColPack::MultiIndex;

int main() {
  std::set<MultiIndex> mset;
  std::map<MultiIndex, int> mmap;
  MultiIndex m1, m2;
  m1.insert(1).insert(1).insert(2);
  m1.dump();
  mset.insert(m1);
  mmap[m1] = 1;

  m2.insert(1).insert(2).insert(2);
  mset.insert(m2);
  mmap[m2] = 2;
  m2.remove(2).insert(1);
  std::cout << mmap[m2] << std::endl;
    
}
