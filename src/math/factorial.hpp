#include <vector>
#include <tuple>
using i64 = long long;

template<class T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<T>> build_factorial(int N) {
  std::vector<T> fact(N);
  std::vector<T> finv(N);
  std::vector<T> inv(N);
  fact[0] = T(1);
  for(int i = 1;i < N;i++) {
    fact[i] = fact[i - 1] * T(i);
  }
  finv[N - 1] = T(1) / fact[N - 1];
  for(int i = N - 1; i --> 0;) {
    finv[i] = finv[i + 1] * T(i + 1);
  }
  for(int i = 0;i < N;i++) {
    inv[i] = fact[i - 1] * finv[i];
  }
  return std::make_tuple(std::move(fact), std::move(finv), std::move(inv));
}
