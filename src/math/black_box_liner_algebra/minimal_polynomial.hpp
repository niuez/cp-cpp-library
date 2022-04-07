#include "../berlekamp_massey.hpp"

#include <vector>

template<class F>
std::vector<F> find_minimal_polynomial(const std::vector<F>& a) {
  std::vector<F> c = berlekamp_massey(a);
  c.insert(c.begin(), -F(1));
  return c;
}

template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_vector(int n, const std::vector<std::vector<F>>& a, NonZeroRandGen rnd) {
  std::vector<F> u(n);
  for(int i = 0; i < n; i++) u[i] = rnd();
  std::vector<F> b(a.size(), F(0));
  for(int i = 0; i < a.size(); i++) {
    for(int j = 0; j < n; j++) {
      b[i] += a[i][j] * u[j];
    }
  }
  return find_minimal_polynomial(b);
}

template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_dense_matrix_pow(const std::vector<std::vector<F>>& a, NonZeroRandGen rnd) {
  int n = a.size();
  std::vector<std::vector<F>> b(n * 2, std::vector<F>(n, F(0)));
  for(int i = 0; i < n; i++) b[0][i] = rnd();
  for(int i = 1; i < 2 * n; i++) {
    auto& bf = b[i - 1];
    auto& nx = b[i];
    for(int j = 0; j < n; j++) {
      for(int k = 0; k < n; k++) {
        nx[j] += a[j][k] * bf[k];
      }
    }
  }
  return find_minimal_polynomial_from_vector(n, b, rnd);
}

// fast for dense matrix
template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_dense_matrix_pow2(const std::vector<std::vector<F>>& a, NonZeroRandGen rnd) {
  int n = a.size();
  std::vector<F> b(n);
  std::vector<F> bf;
  for(int i = 0; i < n; i++) b[i] = rnd();

  std::vector<F> u(n);
  for(int i = 0; i < n; i++) u[i] = rnd();

  std::vector<F> c(n * 2);
  for(int i = 0; i < 2 * n; i++) {
    for(int j = 0; j < n; j++) {
      c[i] += b[j] * u[j];
    }
    if(i + 1 < 2 * n) {
      bf = b;
      for(int j = 0; j < n; j++) {
        b[j] = F(0);
        for(int k = 0; k < n; k++) {
          b[j] += a[j][k] * bf[k];
        }
      }
    }
  }
  return find_minimal_polynomial(c);
}
