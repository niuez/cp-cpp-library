#include <vector>
using i64 = long long;

template<class T>
void build_factorial(std::vector<T>& fact, std::vector<T>& finv, std::vector<T>& inv) {
  std::size_t N = fact.size();
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
}

