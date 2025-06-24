#include <iostream>
#include <array/array.h>

template<typename T, typename Shape>
void print_arr(nda::array<T, Shape> arr) {
  std::cout << "Array:\n";
  // can also access array as arr[{0, 0}]
  std::cout << arr(0, 0) << ", " << arr(0, 1) << ", " << arr(0, 2) << ", " << arr(0, 3) << "\n";
  std::cout << arr(1, 0) << ", " << arr(1, 1) << ", " << arr(1, 2) << ", " << arr(1, 3) << "\n";
  std::cout << arr(2, 0) << ", " << arr(2, 1) << ", " << arr(2, 2) << ", " << arr(2, 3) << "\n";
  std::cout << arr(3, 0) << ", " << arr(3, 1) << ", " << arr(3, 2) << ", " << arr(3, 3) << "\n\n";
};

int main() {
  using arr_shape = nda::shape<
    nda::dim<0, 4, 1>,
    nda::dim<0, 4, 4>
  >;
  nda::array<double, arr_shape> my_arr;

  // can also use nda::fill(my_arr, 1.0) here instead!
  my_arr.for_each_value([](double& x) { x = 1; });
  print_arr(my_arr);



  using iter_shape_for_each_index = nda::shape<
    nda::dim<0, 2>,
    nda::dim<0, 4>
  >;
  std::cout << "Loop order:\n";
  nda::for_each_index(iter_shape_for_each_index(), [&](iter_shape_for_each_index::index_type i) {
    std::cout << std::get<0>(i) << " ";
    std::cout << std::get<1>(i) << ", ";
    my_arr[i] = 2;
  });
  std::cout << "\n";
  print_arr(my_arr);



  using iter_shape_for_all_indices = nda::shape<
    nda::dim<2, 2>,
    nda::dim<1, 3>
  >;
  std::cout << "Loop order:\n";
  nda::for_all_indices(iter_shape_for_all_indices(), [&](int i, int j) {
    std::cout << i << " " << j << ", ";
    my_arr(i, j) = 3;
  });
  std::cout << "\n";
  print_arr(my_arr);


  std::cout << "\nAlternative loop orders:\n\n";

  std::cout << "Shape based (column major in this example):\n";
  nda::for_each_index(iter_shape_for_each_index(), [&](iter_shape_for_each_index::index_type i) {
    std::cout << std::get<0>(i) << " ";
    std::cout << std::get<1>(i) << ", ";
  });
  std::cout << "\n";

  std::cout << "Reverse loop ordering (row major in this example):\n";
  nda::for_each_index<1, 0>(iter_shape_for_each_index(), [&](iter_shape_for_each_index::index_type i) {
    std::cout << std::get<0>(i) << " ";
    std::cout << std::get<1>(i) << ", ";
  });
  std::cout << "\n\n";

  // nda::for_each_index(iter_shape_for_each_index(), [&](auto i) {
  //   int x = i;
  //   std::cout << std::get<0>(i) << " ";
  //   std::cout << std::get<1>(i) << ", ";
  // });


  //auto s = iter_shape_for_each_index();
  //using hey = decltype(s)::index_type;

  std::cout << "Shape based (column major in this example):\n";
  nda::for_all_indices(iter_shape_for_all_indices(), [&](int i, int j) {
    std::cout << i << " " << j << ", ";
  });
  std::cout << "\n";

  std::cout << "Reverse loop ordering (row major in this example):\n";
  nda::for_all_indices<1, 0>(iter_shape_for_all_indices(), [&](int i, int j) {
    std::cout << i << " " << j << ", ";
  });
  std::cout << "\n\n";



  std::cout << "Any permutation works:\n";
  using three_d_shape = nda::shape_of_rank<3>;
  three_d_shape my_three_d_shape(2, 2, 2);
  nda::for_all_indices(my_three_d_shape, [](int i, int j, int k) {
    std::cout << i << ", " << j << ", " << k << "\n";
  });
  std::cout << "\n\nAlternative:\n";
  nda::for_all_indices<1, 0, 2>(my_three_d_shape, [](int i, int j, int k) {
    std::cout << i << ", " << j << ", " << k << "\n";
  });


  std::cout << "\n\nFor loop with auto (and initialisation of shape in array arg):\n";
  nda::array<int, three_d_shape> my_three_d_arr({2, 2, 2});
  for (auto x: my_three_d_arr.x()) {
    for (auto y: my_three_d_arr.y()) {
      for (auto z: my_three_d_arr.z()) {
        std::cout << x << ", " << y << ", " << z << "\n";
      }
    }
  }

  

  std::cout << "\n\nChipping or slicing:\n";
  nda::array<int, nda::shape<
    nda::dim<0, 4, 1>,
    nda::dim<0, 4, 4>
  >> four_by_four;
  nda::for_all_indices(four_by_four.shape(), [&](int i, int j) {
    // column major ordered values, row major would be 4 * i + j
    four_by_four(i, j) = i + 4 * j;
  });

  std::cout << "Array:\n";
  std::cout << four_by_four(0, 0) << ", " << four_by_four(0, 1) << ", " << four_by_four(0, 2) << ", " << four_by_four(0, 3) << "\n";
  std::cout << four_by_four(1, 0) << ", " << four_by_four(1, 1) << ", " << four_by_four(1, 2) << ", " << four_by_four(1, 3) << "\n";
  std::cout << four_by_four(2, 0) << ", " << four_by_four(2, 1) << ", " << four_by_four(2, 2) << ", " << four_by_four(2, 3) << "\n";
  std::cout << four_by_four(3, 0) << ", " << four_by_four(3, 1) << ", " << four_by_four(3, 2) << ", " << four_by_four(3, 3) << "\n\n";

  nda::array_ref_of_rank<int, 1> slice = four_by_four(nda::_, 0);
  std::cout << "Slice view of array:\n";
  slice.for_each_value([](int& val) { std::cout << val << "\n"; });

  auto crop = four_by_four(nda::interval<>{0, 2}, nda::_);
  std::cout << "\nCropped view of array:\n";
  std::cout << crop(0, 0) << ", " << crop(0, 1) << ", " << crop(0, 2) << ", " << crop(0, 3) << "\n";
  std::cout << crop(1, 0) << ", " << crop(1, 1) << ", " << crop(1, 2) << ", " << crop(1, 3) << "\n\n";

  std::cout << "Assignment using shape of crop of array:\n";
  nda::for_all_indices(crop.shape(), [&](int i, int j) {
    four_by_four(i, j) = 100;
  });
  std::cout << "Array:\n";
  std::cout << four_by_four(0, 0) << ", " << four_by_four(0, 1) << ", " << four_by_four(0, 2) << ", " << four_by_four(0, 3) << "\n";
  std::cout << four_by_four(1, 0) << ", " << four_by_four(1, 1) << ", " << four_by_four(1, 2) << ", " << four_by_four(1, 3) << "\n";
  std::cout << four_by_four(2, 0) << ", " << four_by_four(2, 1) << ", " << four_by_four(2, 2) << ", " << four_by_four(2, 3) << "\n";
  std::cout << four_by_four(3, 0) << ", " << four_by_four(3, 1) << ", " << four_by_four(3, 2) << ", " << four_by_four(3, 3) << "\n\n";



  constexpr nda::index_t x_split = 2;
  const nda::index_t y_split = 2;
  
  // produce intervals with x_split/y_split number of elements in each interval
  for (auto x_interval: nda::split(four_by_four.x(), x_split)) {
    for (auto y_interval: nda::split(four_by_four.y(), y_split)) {
      auto tile = four_by_four(x_interval, y_interval);
      std::cout << "Printing tile:\n";
      for (auto x: tile.x()) {
        for (auto y: tile.y()) {
          std::cout << x << ", " << y << "\n";
        }
      }
    }
  }
}
