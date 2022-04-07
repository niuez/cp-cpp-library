#include "minimal_polynomial.hpp"
#include <vector>

template<class F, class NonZeroRandGen>
F fast_determinant(std::vector<std::vector<F>> a, NonZeroRandGen rng) {
  int n = a.size();
  std::vector<F> d(n);
  F d_det(1);
  for(int i = 0; i < n; i++) {
    d[i] = rng();
    d_det *= d[i];
  }
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      a[i][j] = a[i][j] * d[j];
    }
  }
  auto c = find_minimal_polynomial_from_dense_matrix_pow2(a, rng);
  F det = c.back() / c.front();
  det = n & 1 ? -det : det;
  return det / d_det;
}

