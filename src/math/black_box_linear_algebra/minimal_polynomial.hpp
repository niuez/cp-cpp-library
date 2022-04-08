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
std::vector<F> find_minimal_polynomial_from_dense_matrix_pow_b(const std::vector<std::vector<F>>& a, std::vector<F> b, NonZeroRandGen rnd) {
  int n = a.size();
  std::vector<F> bf;

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

// fast for dense matrix
template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_dense_matrix_pow(const std::vector<std::vector<F>>& a, NonZeroRandGen rnd) {
  int n = a.size();
  std::vector<F> b(n);
  for(int i = 0; i < n; i++) b[i] = rnd();
  return find_minimal_polynomial_from_dense_matrix_pow_b(a, std::move(b), rnd);
}

#include <tuple>

template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_sparse_matrix_pow_b(const std::vector<std::tuple<int, int, F>>& a, std::vector<F> b, int n, NonZeroRandGen rnd) {
  std::vector<F> bf;

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
      }
      for(auto& [j, k, v]: a) {
        b[j] += v * bf[k];
      }
    }
  }
  return find_minimal_polynomial(c);
}

template<class F, class NonZeroRandGen>
std::vector<F> find_minimal_polynomial_from_sparse_matrix_pow(const std::vector<std::tuple<int, int, F>>& a, int n, NonZeroRandGen rnd) {
  std::vector<F> b(n);
  for(int i = 0; i < n; i++) b[i] = rnd();
  return find_minimal_polynomial_from_sparse_matrix_pow_b(a, std::move(b), n, rnd);
}
