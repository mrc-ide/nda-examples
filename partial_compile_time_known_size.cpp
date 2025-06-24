#include <array/array.h>
#include <iostream>

int main(int argc, const char * argv[]) {
  int runtime_dim = atoi(argv[1]);

  using runtime_dim2_shape = nda::shape<
    nda::dim<0, 4, 1>,
    nda::dim<0, nda::dynamic, 4>
  >;

  // runtime_dim2_shape my_dim2_shape(3, runtime_dim);
  nda::array<int, runtime_dim2_shape> my_arr(runtime_dim2_shape({ 4, runtime_dim }));

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < runtime_dim; j++) {
      my_arr(i, j) = i * 1000 + j;
    }
  }

  std::cout << "My array: \n";
  std::cout << my_arr(0, 0) << ", " << my_arr(0, 1) << ", " << my_arr(0, 2) << ", " << my_arr(0, 3) << ", " << my_arr(0, 4) << ", " << my_arr(0, 5) << "...\n";
  std::cout << my_arr(1, 0) << ", " << my_arr(1, 1) << ", " << my_arr(1, 2) << ", " << my_arr(1, 3) << ", " << my_arr(1, 4) << ", " << my_arr(1, 5) << "...\n\n";

  return 0;
}
