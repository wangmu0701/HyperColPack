#include "ColPack/ColPackHeaders.h"

using ColPack::GraphColoringInterface;


void test_distance_one(const char * const s_Ordering,
                       const char * const file_name) {
  GraphColoringInterface* g =
    new GraphColoringInterface(SRC_FILE, file_name, "MM");
  g->Coloring(s_Ordering, "DISTANCE_ONE");
  int num_colors = g->GetVertexColorCount();
  std::cout <<"    "<< s_Ordering << " : " << num_colors << std::endl;
  delete g;
}
int main(int argc, char* argv[]) {
  std::cout << "Distance One Coloring ...." << std::endl;
  test_distance_one("NATURAL", argv[1]);
  test_distance_one("LARGEST_FIRST", argv[1]);
  test_distance_one("DYNAMIC_LARGEST_FIRST", argv[1]);
  //test_distance_one("SMALLEST_LAST", argv[1]);
  return 0;
}
