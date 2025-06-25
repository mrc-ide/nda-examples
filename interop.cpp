#include <cpp11.hpp>
#include <array/array.h>

[[cpp11::register]]
void show_matrix(cpp11::sexp x) {
  auto dim = cpp11::as_cpp<cpp11::integers>(x.attr("dim"));
  if (dim.size() != 2) {
    cpp11::stop("Expected a matrix\n");
  }

  const size_t nr = dim[0];
  const size_t nc = dim[1];

  nda::shape_of_rank<2> dim_a{nr, nc};
  nda::array_ref_of_rank<const double, 2> a(REAL(x), dim_a);

  for (size_t i = 0; i < nr; ++i) {
    for (size_t j = 0; j < nc; ++j) {
      Rprintf("%.1f ", a(i, j));
    }
    Rprintf("\n");
  }
}

[[cpp11::register]]
void show_array3(cpp11::sexp x) {
  auto dim = cpp11::as_cpp<cpp11::integers>(x.attr("dim"));
  if (dim.size() != 3) {
    cpp11::stop("Expected a 3d array\n");
  }
  const size_t n1 = dim[0];
  const size_t n2 = dim[1];
  const size_t n3 = dim[2];

  nda::shape_of_rank<3> dim_a{n1, n2, n3};
  nda::array_ref_of_rank<const double, 3> a(REAL(x), dim_a);

  for (size_t i = 0; i < n1; ++i) {
    Rprintf("x[%d, , ]:\n", static_cast<int>(i + 1));
    for (size_t j = 0; j < n2; ++j) {
      for (size_t k = 0; k < n3; ++k) {
        Rprintf("%.1f ", a(i, j, k));
      }
      Rprintf("\n");
    }
  }
}
