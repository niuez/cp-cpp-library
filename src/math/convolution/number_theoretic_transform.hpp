#include "../modint.hpp"
#include <vector>
using namespace std;
constexpr i64 NTT_PRIMES[][2] = {
    {1224736769, 3}, // 2^24 * 73 + 1,
    {1053818881, 7}, // 2^20 * 3 * 5 * 67 + 1
    {1051721729, 6}, // 2^20 * 17 * 59 + 1
    {1045430273, 3}, // 2^20 * 997 + 1
    {1012924417, 5}, // 2^21 * 3 * 7 * 23 + 1
    {1007681537, 3}, // 2^20 * 31^2 + 1
    {1004535809, 3}, // 2^21 * 479 + 1
    {998244353, 3},  // 2^23 * 7 * 17 + 1
    {985661441, 3},  // 2^22 * 5 * 47 + 1
    {976224257, 3},  // 2^20 * 7^2 * 19 + 1
    {975175681, 17}, // 2^21 * 3 * 5 * 31 + 1
    {962592769, 7},  // 2^21 * 3^3 * 17 + 1
    {950009857, 7},  // 2^21 * 4 * 151 + 1
    {943718401, 7},  // 2^22 * 3^2 * 5^2 + 1
    {935329793, 3},  // 2^22 * 223 + 1
    {924844033, 5},  // 2^21 * 3^2 * 7^2 + 1
    {469762049, 3},  // 2^26 * 7 + 1
    {167772161, 3},  // 2^25 * 5 + 1
};

template<const i64 mod, const i64 primitive>
vector<modint<mod>> number_theoretic_transform(vector<modint<mod>> a) {
  i64 n = a.size();
  for(i64 s = n >> 1; s >= 1; s >>= 1) {
    modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (s << 1));
    for(i64 i = 0; i < n; i += (s << 1)) {
      modint<mod> zi = 1;
      for(i64 j = 0;j < s;j++) {
        modint<mod> t = a[i + j] - a[s + i + j];
        a[i + j] += a[s + i + j];
        a[s + i + j] = t * zi;
        zi = zi * zeta;
      }
    }
  }
  return a;
}

template<const i64 mod, const i64 primitive>
vector<modint<mod>> inverse_number_theoretic_transform(vector<modint<mod>> a) {
  i64 n = a.size();
  for(i64 s = 1; s < n; s <<= 1) {
    modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (s << 1)).pow(mod - 2);
    for(i64 i = 0; i < n; i += (s << 1)) {
      modint<mod> zi = 1;
      for(i64 j = 0;j < s;j++) {
        modint<mod> t = a[s + i + j] * zi;
        a[s + i + j] = a[i + j] - t;
        a[i + j] = a[i + j] + t;
        zi = zi * zeta;
      }
    }
  }
  auto inv_n = modint<mod>(n).pow(mod - 2);
  for(int i = 0;i < n;i++) a[i] *= inv_n;
  return a;
}
