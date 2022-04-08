#include "../fast_kitamasa.hpp"
#include <vector>
#include <tuple>
template<class F, class FM, class NonZeroRandGen>
std::vector<F> bbla_dense_matrix_pow(const std::vector<std::vector<F>>& a, std::vector<F> b, long long r, NonZeroRandGen rnd) {
  int n = a.size();
  auto c = find_minimal_polynomial_from_dense_matrix_pow_b(a, b, rnd);
  auto d = fast_kitamasa<F, FM>(std::move(c), r);
  std::vector<F> ans(n);
  std::vector<F> bf;
  for(int i = 0; i < d.size(); i++) {
    for(int j = 0; j < n; j++) {
      ans[j] += d[d.size() - 1 - i] * b[j];
    }
    if(i + 1 < d.size()) {
      bf = b;
      for(int j = 0; j < n; j++) {
        b[j] = F(0);
        for(int k = 0; k < n; k++) {
          b[j] += a[j][k] * bf[k];
        }
      }
    }
  }
  return ans;
}

template<class F, class FM, class NonZeroRandGen>
std::vector<F> bbla_sparse_matrix_pow(const std::vector<std::tuple<int, int, F>>& a, std::vector<F> b, long long r, NonZeroRandGen rnd) {
  int n = b.size();
  auto c = find_minimal_polynomial_from_sparse_matrix_pow_b(a, b, n, rnd);
  auto d = fast_kitamasa<F, FM>(std::move(c), r);
  std::vector<F> ans(n);
  std::vector<F> bf;
  for(int i = 0; i < d.size(); i++) {
    for(int j = 0; j < n; j++) {
      ans[j] += d[d.size() - 1 - i] * b[j];
    }
    if(i + 1 < d.size()) {
      bf = b;
      for(int j = 0; j < n; j++) {
        b[j] = F(0);
      }
      for(auto& [j, k, v]: a) {
        b[j] += v * bf[k];
      }
    }
  }
  return ans;
}
