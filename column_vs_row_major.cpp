#include <array/array.h>
#include <iostream>

int main() {
  double arr[2 * 3];
  std::cout << "\nArray: \n";
  for (int i = 0; i < 2 * 3; i++) {
    arr[i] = i;
    std::cout << i << " ";
  }
  std::cout << "\n\n";

  using col_major_shape = nda::shape<
    nda::dim<0, 2, 1>,
    nda::dim<0, 3, 2>
  >;
  nda::array_ref<double, col_major_shape> col_major_matrix(arr);

  std::cout << "Column major interpretation: \n";
  std::cout << col_major_matrix(0, 0) << col_major_matrix(0, 1) << col_major_matrix(0, 2) << "\n";
  std::cout << col_major_matrix(1, 0) << col_major_matrix(1, 1) << col_major_matrix(1, 2) << "\n\n";


  using row_major_shape = nda::shape<
    nda::dim<0, 2, 3>,
    nda::dim<0, 3, 1>
  >;

  nda::array_ref<double, row_major_shape> row_major_matrix(arr);

  std::cout << "Row major interpretation: \n";
  std::cout << row_major_matrix(0, 0) << row_major_matrix(0, 1) << row_major_matrix(0, 2) << "\n";
  std::cout << row_major_matrix(1, 0) << row_major_matrix(1, 1) << row_major_matrix(1, 2) << "\n\n";

  return 0;
}
