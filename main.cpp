#include <unsupported/Eigen/CXX11/Tensor>
#include <array/array.h>
#include <array/ein_reduce.h>
#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>

int main(int argc, const char * argv[]) {

  double rand_data1[10 * 20 * 30];  
  double rand_data2[10 * 40 * 30];
  double rand_data3[10];
  
  double lower_bound = 0;
  double upper_bound = 100;
  
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  std::default_random_engine re;
  
  for (int i = 0; i < 10 * 20 * 30; i++) {
    rand_data1[i] = unif(re);
  }
  for (int i = 0; i < 10 * 40 * 30; i++) {
    rand_data2[i] = unif(re);
  }
  for (int i = 0; i < 10; i++) {
    rand_data3[i] = unif(re);
  }

  Eigen::TensorMap<Eigen::Tensor<double, 3>> t1(rand_data1, 10, 20, 30);
  Eigen::TensorMap<Eigen::Tensor<double, 3>> t2(rand_data2, 10, 40, 30);
  Eigen::TensorMap<Eigen::Tensor<double, 1>> t3(rand_data3, 10);

  using shape1 = nda::shape<
    nda::dim<0, 10, 1>,
    nda::dim<0, 20, 10>,
    nda::dim<0, 30, 200>
  >;

  using shape2 = nda::shape<
    nda::dim<0, 10, 1>,
    nda::dim<0, 40, 10>,
    nda::dim<0, 30, 400>
  >;

  using shape3 = nda::shape<
    nda::dim<0, 10, 1>
  >;

  nda::array_ref<double, shape1> n1(rand_data1);
  nda::array_ref<double, shape2> n2(rand_data2);
  nda::array_ref<double, shape3> n3(rand_data3);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  
  using shape_add = nda::shape<
    nda::dim<0, 20, 1>,
    nda::dim<0, 40, 20>
  >;
  
  auto nda1 = high_resolution_clock::now();

  nda::array<double, shape_add> added_nda1;
  added_nda1.for_each_value([](double& value) {
    value = 0;
  });

  for (int i = 0; i < 10; i++) {
    for (int k = 0; k < 30; k++) {
      for (int j = 0; j < 20; j++) {
        for (int w = 0; w < 40; w++) {
          added_nda1(j, w) += n1(i, j, k) * n2(i, w, k) + n3(i);
        }
      }
    }
  }

  auto nda2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_nda = nda2 - nda1;
  std::cout << "NDA for loop took: " << ms_double_nda.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << added_nda1(0, 0) << ", ";
  // std::cout << added_nda1(0, 1) << ", ";
  // std::cout << added_nda1(0, 2) << ", ";
  // std::cout << added_nda1(0, 3) << ", " << "...\n";
  // std::cout << added_nda1(1, 0) << ", ";
  // std::cout << added_nda1(1, 1) << ", ";
  // std::cout << added_nda1(1, 2) << ", ";
  // std::cout << added_nda1(1, 3) << ", " << "...\n";
  // std::cout << added_nda1(2, 0) << ", ";
  // std::cout << added_nda1(2, 1) << ", ";
  // std::cout << added_nda1(2, 2) << ", ";
  // std::cout << added_nda1(2, 3) << ", " << "...\n\n";





  auto nda11 = high_resolution_clock::now();

  nda::array<double, shape_add> added_nda2;
  added_nda2.for_each_value([](double& value) {
    value = 0;
  });

  using shape4d_iter = nda::shape<nda::dim<0, 10, 1>, nda::dim<0, 30, 10>, nda::dim<0, 20, 300>, nda::dim<0, 40, 6000>>;
  nda::for_all_indices(shape4d_iter(), [&](int i, int k, int j, int w) {
    added_nda2(j, w) += n1(i, j, k) * n2(i, w, k) + n3(i);
  });

  auto nda12 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_nda1 = nda12 - nda11;
  std::cout << "NDA for all indices took: " << ms_double_nda1.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << added_nda2(0, 0) << ", ";
  // std::cout << added_nda2(0, 1) << ", ";
  // std::cout << added_nda2(0, 2) << ", ";
  // std::cout << added_nda2(0, 3) << ", " << "...\n";
  // std::cout << added_nda2(1, 0) << ", ";
  // std::cout << added_nda2(1, 1) << ", ";
  // std::cout << added_nda2(1, 2) << ", ";
  // std::cout << added_nda2(1, 3) << ", " << "...\n";
  // std::cout << added_nda2(2, 0) << ", ";
  // std::cout << added_nda2(2, 1) << ", ";
  // std::cout << added_nda2(2, 2) << ", ";
  // std::cout << added_nda2(2, 3) << ", " << "...\n\n";




  auto nda111 = high_resolution_clock::now();

  nda::array<double, shape_add> added_nda3;
  added_nda3.for_each_value([](double& value) {
    value = 0;
  });

  enum { ii = 0, jj = 1, kk = 2, ww = 3 };
  nda::ein_reduce(nda::ein<jj, ww>(added_nda3) += nda::ein<ii, jj, kk>(n1) * nda::ein<ii, ww, kk>(n2) + nda::ein<ii>(n3));

  auto nda112 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_nda11 = nda112 - nda111;
  std::cout << "NDA einstein summation took: " << ms_double_nda11.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << added_nda3(0, 0) << ", ";
  // std::cout << added_nda3(0, 1) << ", ";
  // std::cout << added_nda3(0, 2) << ", ";
  // std::cout << added_nda3(0, 3) << ", " << "...\n";
  // std::cout << added_nda3(1, 0) << ", ";
  // std::cout << added_nda3(1, 1) << ", ";
  // std::cout << added_nda3(1, 2) << ", ";
  // std::cout << added_nda3(1, 3) << ", " << "...\n";
  // std::cout << added_nda3(2, 0) << ", ";
  // std::cout << added_nda3(2, 1) << ", ";
  // std::cout << added_nda3(2, 2) << ", ";
  // std::cout << added_nda3(2, 3) << ", " << "...\n\n";


  
  // Eigen for loops
  auto eigen1 = high_resolution_clock::now();

  Eigen::TensorFixedSize<double, Eigen::Sizes<20, 40>> added;
  added.setZero();

  for (int i = 0; i < 10; i++) {
    for (int k = 0; k < 30; k++) {
      for (int j = 0; j < 20; j++) {
        for (int w = 0; w < 40; w++) {
          added(j, w) += t1(i, j, k) * t2(i, w, k) + t3(i);
        }
      }
    }
  }

  auto eigen2 = high_resolution_clock::now();

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = eigen2 - eigen1;
  std::cout << "Eigen for loops took: " << ms_double.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << added(0, 0) << ", ";
  // std::cout << added(0, 1) << ", ";
  // std::cout << added(0, 2) << ", ";
  // std::cout << added(0, 3) << ", " << "...\n";
  // std::cout << added(1, 0) << ", ";
  // std::cout << added(1, 1) << ", ";
  // std::cout << added(1, 2) << ", ";
  // std::cout << added(1, 3) << ", " << "...\n";
  // std::cout << added(2, 0) << ", ";
  // std::cout << added(2, 1) << ", ";
  // std::cout << added(2, 2) << ", ";
  // std::cout << added(2, 3) << ", " << "...\n\n";







  int runtime_dim = atoi(argv[1]);

  constexpr int dim0 = 10;
  constexpr int dim1 = 20;

  Eigen::Tensor<double, 3> ot1(dim0, dim1, runtime_dim);
  ot1.setZero();
  Eigen::Tensor<double, 3> ot2(dim0, dim1, runtime_dim);
  ot2.setZero();
  Eigen::Tensor<double, 3> ot3(dim0, dim1, runtime_dim);
  ot3.setZero();

  using shape_on1 = nda::shape<
    nda::dim<0, dim0, 1>,
    nda::dim<0, dim1, dim0>,
    nda::dim<nda::dynamic, nda::dynamic, dim0 * dim1>
  >;

  shape_on1 runtime_shape(dim0, dim1, runtime_dim);

  nda::array<double, shape_on1> on1(runtime_shape);
  nda::array<double, shape_on1> on2(runtime_shape);
  nda::array<double, shape_on1> on3(runtime_shape);

  on1.for_each_value([](double& value) { value = 0; });
  on2.for_each_value([](double& value) { value = 0; });
  on3.for_each_value([](double& value) { value = 0; });

  for (int i = 0; i < dim0; i++) {
    for (int j = 0; j < dim1; j++) {
      for (int k = 0; k < runtime_dim; k++) {
        auto rand1 = unif(re);
        auto rand2 = unif(re);
        ot1(i, j, k) = rand1;
        ot2(i, j, k) = rand2;
        on1(i, j, k) = rand1;
        on2(i, j, k) = rand2;
      }
    }
  }

  auto eigen_owned_1 = high_resolution_clock::now();

  for (int i = 0; i < dim0; i++) {
    for (int j = 0; j < dim1; j++) {
      for (int k = 0; k < runtime_dim; k++) {
        ot3(i, j, k) = ot1(i, j, k) * ot2(i, j, k);
      }
    }
  }

  auto eigen_owned_2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double_owned_eigen = eigen_owned_2 - eigen_owned_1;
  std::cout << "Eigen owned with 1 runtime dim for loops took: " << ms_double_owned_eigen.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << ot3(0, 0, 0) << ", ";
  // std::cout << ot3(0, 0, 1) << ", ";
  // std::cout << ot3(0, 0, 2) << ", ";
  // std::cout << ot3(0, 0, 3) << ", " << "...\n";
  // std::cout << ot3(0, 1, 0) << ", ";
  // std::cout << ot3(0, 1, 1) << ", ";
  // std::cout << ot3(0, 1, 2) << ", ";
  // std::cout << ot3(0, 1, 3) << ", " << "...\n\n";
  // std::cout << ot3(1, 0, 0) << ", ";
  // std::cout << ot3(1, 0, 1) << ", ";
  // std::cout << ot3(1, 0, 2) << ", ";
  // std::cout << ot3(1, 0, 3) << ", " << "...\n";
  // std::cout << ot3(1, 1, 0) << ", ";
  // std::cout << ot3(1, 1, 1) << ", ";
  // std::cout << ot3(1, 1, 2) << ", ";
  // std::cout << ot3(1, 1, 3) << ", " << "...\n\n\n";





  auto nda_owned_1 = high_resolution_clock::now();

  // for (int i = 0; i < dim0; i++) {
  //   for (int j = 0; j < dim1; j++) {
  //     for (int k = 0; k < runtime_dim; k++) {
  //       on3(i, j, k) = on1(i, j, k) * on2(i, j, k);
  //     }
  //   }
  // }

  nda::for_all_indices(runtime_shape, [&](int i, int j, int k) {
    on3(i, j, k) = on1(i, j, k) * on2(i, j, k);
  });

  // nda::ein_reduce(nda::ein<ii, jj, kk>(on3) = nda::ein<ii, jj, kk>(on1) * nda::ein<ii, jj, kk>(on2));

  auto nda_owned_2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double_owned_nda = nda_owned_2 - nda_owned_1;
  std::cout << "NDA owned with 1 runtime dim for loop took: " << ms_double_owned_nda.count() << "ms\n";

  // std::cout << "Result:\n";
  // std::cout << on3(0, 0, 0) << ", ";
  // std::cout << on3(0, 0, 1) << ", ";
  // std::cout << on3(0, 0, 2) << ", ";
  // std::cout << on3(0, 0, 3) << ", " << "...\n";
  // std::cout << on3(0, 1, 0) << ", ";
  // std::cout << on3(0, 1, 1) << ", ";
  // std::cout << on3(0, 1, 2) << ", ";
  // std::cout << on3(0, 1, 3) << ", " << "...\n\n";
  // std::cout << on3(1, 0, 0) << ", ";
  // std::cout << on3(1, 0, 1) << ", ";
  // std::cout << on3(1, 0, 2) << ", ";
  // std::cout << on3(1, 0, 3) << ", " << "...\n";
  // std::cout << on3(1, 1, 0) << ", ";
  // std::cout << on3(1, 1, 1) << ", ";
  // std::cout << on3(1, 1, 2) << ", ";
  // std::cout << on3(1, 1, 3) << ", " << "...\n\n\n";

  return 0;
}
