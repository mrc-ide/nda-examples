Sys.setenv("PKG_CXXFLAGS" = sprintf("-I%s", getwd()))
cpp11::cpp_source("interop.cpp", cxx_std = "CXX17")

# This surprises me, because we've used the column-major data provided by R
m <- matrix(as.numeric(1:12), 3, 4)
show_matrix(m)

a <- array(as.numeric(1:24), c(2, 3, 4))
show_array3(a)
