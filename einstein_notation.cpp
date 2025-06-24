#include <array/array.h>
#include <array/ein_reduce.h>
#include <iostream>

void print_four_by_four(nda::array<double, nda::shape_of_rank<2>> arr) {
  std::cout << "Matrix:\n";
  std::cout << arr(0, 0) << ", " << arr(0, 1) << ", " << arr(0, 2) << ", " << arr(0, 3) << "\n";
  std::cout << arr(1, 0) << ", " << arr(1, 1) << ", " << arr(1, 2) << ", " << arr(1, 3) << "\n";
  std::cout << arr(2, 0) << ", " << arr(2, 1) << ", " << arr(2, 2) << ", " << arr(2, 3) << "\n";
  std::cout << arr(3, 0) << ", " << arr(3, 1) << ", " << arr(3, 2) << ", " << arr(3, 3) << "\n\n";
}

enum { i = 0, j = 1, k = 2 };

int main() {
  nda::array<double, nda::shape_of_rank<2>> matrix({4, 4});
  nda::fill(matrix, 1.0);
  nda::for_all_indices(matrix.shape(), [&](int x, int y) {
    if (x == y) { matrix(x, y) = x + 4; }
  });

  print_four_by_four(matrix);



  double trace = 0;

  nda::ein_reduce(nda::ein(trace) += nda::ein<i, i>(matrix));
  
  std::cout << "Trace is: " << trace << "\n\n";



  nda::array<double, nda::shape_of_rank<2>> upper_triangular({4, 4});
  nda::fill(upper_triangular, 0.0);

  auto is_upper = [](int x, int y) { return x <= y; };
  nda::ein_reduce(nda::ein<i, j>(upper_triangular) = nda::ein<i, j>(is_upper) * nda::ein<i, j>(matrix));
  
  std::cout << "Upper triangular matrix:\n";
  print_four_by_four(upper_triangular);


  nda::for_all_indices(matrix.shape(), [&](int x, int y) {
    matrix(x, y) = x + 4 * y;
  });
  std::cout << "New Matrix:\n";
  print_four_by_four(matrix);


  nda::array<double, nda::shape_of_rank<1>> maxes_col({4});
  nda::fill(maxes_col, 0.0);
  nda::array<double, nda::shape_of_rank<1>> maxes_row({4});
  nda::fill(maxes_row, 0.0);

  nda::ein_reduce(nda::ein<i>(maxes_col) = nda::max(nda::ein<i>(maxes_col), nda::ein<j, i>(matrix)));
  std::cout << "Max along columns:\n";
  std::cout << maxes_col(0) << ", " << maxes_col(1) << ", " << maxes_col(2) << ", " << maxes_col(3) << "\n";

  nda::ein_reduce(nda::ein<i>(maxes_row) = nda::max(nda::ein<i>(maxes_row), nda::ein<i, j>(matrix)));
  std::cout << "Max along rows:\n";
  std::cout << maxes_row(0) << "\n" << maxes_row(1) << "\n" << maxes_row(2) << "\n" << maxes_row(3) << "\n";


  std::cout << "\nCan multiply two matrices:\n";
  nda::array<double, nda::shape_of_rank<2>> A({4, 4});
  nda::array<double, nda::shape_of_rank<2>> B({4, 4});
  nda::array<double, nda::shape_of_rank<2>> C({4, 4});

  nda::for_all_indices(A.shape(), [&](int x, int y) {
    A(x, y) = (x == y) ? -1 : 0;
    B(x, y) = x + 4 * y;
    C(x, y) = 0;
  });

  std::cout << "\nA:\n";
  print_four_by_four(A);
  std::cout << "\nB:\n";
  print_four_by_four(B);
  std::cout << "\nC:\n";
  print_four_by_four(C);

  using namespace nda;
  // loops over k as repeated so need the += to add k iterations
  ein_reduce(ein<i, j>(C) += ein<i, k>(A) * ein<k, j>(B));

  std::cout << "\nResult:\n";
  print_four_by_four(C);



  nda::array<double, nda::shape_of_rank<2>> CT({4, 4});
  ein_reduce(ein<i, j>(CT) = ein<j, i>(C));

  std::cout << "\nTranspose of C:\n";
  print_four_by_four(CT);
  
  return 0;
}
