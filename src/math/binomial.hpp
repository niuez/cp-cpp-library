#include <vector>
#include <tuple>
using i64 = long long;

template<class T>
struct binomial {
  const std::vector<T>& fact;
  const std::vector<T>& finv;
  binomial(const std::vector<T>& fact, const std::vector<T>& finv): fact(fact), finv(finv) {}

  T operator()(i64 n, i64 r) const {
    if(0 <= r && r <= n) return fact[n] * finv[n - r] * finv[r];
    else return T(0);
  }
};
