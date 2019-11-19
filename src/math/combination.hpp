#include <vector>
using namespace std;
using i64 = long long;


template<class T>
struct combination {
  vector<T> fact;
  vector<T> inv;

  combination(i64 n) : fact(n), inv(n) {
    fact[0] = T(1);
    for(i64 i = 1;i < n;i++)
      fact[i] = fact[i - 1] * T(i);
    inv[n - 1] = T(1) / fact(n - 1);
    for(i64 i = n - 1;i --> 0;)
      inv[i] = inv[i + 1] * T(i + 1);
  }

  T binom(i64 n, i64 k) const {
    if(k < 0 || n < k) return T(0);
    else return fact[n] * inv[k] * inv[n - k];
  }

   T H(i64 n, i64 k) const {
    return binom(n + k - 1, k);
  };

   T factor(i64 i) const { return fact[i]; }
};
