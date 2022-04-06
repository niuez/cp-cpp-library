#include <vector>
#include <tuple>
using i64 = long long;

template<class T>
struct factorial {
  std::vector<T> fact;
  std::vector<T> finv;
  std::vector<T> inv;

  void build(int N) {
    fact.resize(N);
    finv.resize(N);
    inv.resize(N);
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

  T binom(int n, int r) const {
    if(0 <= r && r <= n) return fact[n] * finv[n - r] * finv[r];
    else return T(0);
  }

  std::tuple<const std::vector<T>&, const std::vector<T>&, const std::vector<T>&> get() const {
    return std::tuple<const std::vector<T>&, const std::vector<T>&, const std::vector<T>&>(fact, finv, inv);
  }
};
