#ifndef HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_
#define HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_

#include <map>
#include <set>
#include <vector>

#include "hyper_graph.hpp"

#define MAX_ALTERNATIVE_ATTEMPTS 10

namespace HyperColPack {

class HyperSymmetricPartition {
 public:
  // Constructor
  HyperSymmetricPartition(const HyperGraph& g);

  // run the coloring procedure
  void try_coloring();

  // get the next vertex that we should try to color
  // -1 : we done the coloring!
  int get_next_v();
  
  // 1: e is adjacent to v, 
  // 2: on slice [c], _compressed_index_set[e/v] should be zero
  // 3: do this for ALL UNRECOVERED edges adjacent to v
  bool check_recoverable(std::shared_ptr<HyperEdge> e, int v, int color);

  // 1: e is adjacent to v
  // 2: on slice [c], _recover_index_set[e/v] should be zero
  // 3: do this for ALL edges adjacent to v
  bool check_conflict(std::shared_ptr<HyperEdge> e, int v, int color);
  
  // something will make e UNrecoverable on v
  // can we find another colored vertex that make e recoverable?
  // If yes, use that vertex to recover e, so we can resolve some conflicts
  bool try_another_recover_v(std::shared_ptr<HyperEdge> e, int v);

  // utilities
  int get_num_of_colors() const {return _num_of_colors;}
  void dump() const;
  bool check() const;
 private:
  // number of colored vertices
  int _colored_v_num;

  int _num_of_colors;

  // map v -> adjacent edges
  std::map<int, std::set<std::shared_ptr<HyperEdge>>> _adjacent_edges;

  // all unrecovered_edges;
  std::set<std::shared_ptr<HyperEdge>> _unrecovered_edges;
  std::set<std::shared_ptr<HyperEdge>> _all_edges;
  
  // color[v];
  std::vector<int> v_colors;

  // number of attempts that this edge have tried for an alternative recover
  std::map<std::shared_ptr<HyperEdge>, int> _alternative_attempts;

  // this is tricky, otherwise there will be a bug
  std::set<std::shared_ptr<HyperEdge>> _alternative_edge_set;

  // map HyperEdge -> recovery index
  std::map<std::shared_ptr<HyperEdge>, int> _recovery_index;

  // the non-zero index set on each slice
  // a new recovery index can NOT overlap with any of these
  std::map<int, std::multiset<MultiIndexSet>> _compressed_index_set;

  // the recovery index set on each slice
  // a new non-recovery index should NOT overlap with any of these
  std::map<int, std::map<MultiIndexSet, std::shared_ptr<HyperEdge>>> _recover_index_set;
};

} // namespace HyperColPack

#endif // HYPERCOLPACK_HYPER_SYMMETRIC_PARTITION_H_
