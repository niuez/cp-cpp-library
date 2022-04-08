#include "../modint.hpp"
#include <vector>
using namespace std;
using i64 = long long;


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
vector<modint<mod>> number_theoretic_transform4(vector<modint<mod>> a) {
  i64 n = a.size();
  vector<modint<mod>> b(a.size());
  auto unit_i = modint<mod>(primitive).pow((mod - 1) / 4);
  for(i64 s = 1, m = n; s < n; s <<= 1, m >>= 1) {
    if(m == 2) {
      for(i64 j = 0;j < s;j++) {
        auto x = a[j + 0];
        auto y = a[j + s];
        b[j + 0] = x + y;
        b[j + s] = x - y;
      }
    }
    else {
      modint<mod> zi1 = 1;
      modint<mod> zi2 = 1;
      modint<mod> zi3 = 1;
      i64 m1 = m >> 2;
      i64 m2 = m >> 1;
      i64 m3 = m1 | m2;
      modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / m);
      for(i64 i = 0;i < m1;i++) {
        for(i64 j = 0;j < s;j++) {
          auto w = a[j + s * (i + 0)];
          auto x = a[j + s * (i + m1)];
          auto y = a[j + s * (i + m2)];
          auto z = a[j + s * (i + m3)];
          auto wy1 = w + y;
          auto wy2 = w - y;
          auto xz1 = x + z;
          auto xz2 = (x - z) * unit_i;
          b[j + s * (4 * i + 0)] =  wy1 + xz1;
          b[j + s * (4 * i + 1)] = (wy2 + xz2) * zi1;
          b[j + s * (4 * i + 2)] = (wy1 - xz1) * zi2;
          b[j + s * (4 * i + 3)] = (wy2 - xz2) * zi3;
        }
        zi1 = zi1 * zeta;
        zi2 = zi1 * zi1;
        zi3 = zi1 * zi2;
      }
      s <<= 1;
      m >>= 1;
    }
    swap(a, b);
  }
  return a;
}

template<const i64 mod, const i64 primitive>
vector<modint<mod>> inverse_number_theoretic_transform4(vector<modint<mod>> a) {
  i64 n = a.size();
  vector<modint<mod>> b(a.size());
  auto unit_i = modint<mod>(primitive).pow((mod - 1) / 4).inv();
  i64 s = n;
  i64 m = 1;
  if(__builtin_ctzll(n) & 1) {
    s >>= 1;
    m <<= 1;
    for(i64 j = 0;j < s;j++) {
      auto x = a[j + 0];
      auto y = a[j + s];
      b[j + 0] = x + y;
      b[j + s] = x - y;
    }
    swap(a, b);
  }
  for(; s >>= 2, m <<= 2, s >= 1;) {
    {
      modint<mod> zi1 = 1;
      modint<mod> zi2 = 1;
      modint<mod> zi3 = 1;
      i64 m1 = m >> 2;
      i64 m2 = m >> 1;
      i64 m3 = m1 | m2;
      modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / m).inv();
      for(i64 i = 0;i < m1;i++) {
        for(i64 j = 0;j < s;j++) {
          auto w = a[j + s * (4 * i + 0)];
          auto x = a[j + s * (4 * i + 1)] * zi1;
          auto y = a[j + s * (4 * i + 2)] * zi2;
          auto z = a[j + s * (4 * i + 3)] * zi3;
          auto wy1 = w + y;
          auto wy2 = x + z;
          auto xz1 = w - y;
          auto xz2 = (x - z) * unit_i;
          b[j + s * (i + 0)]  = wy1 + wy2;
          b[j + s * (i + m1)] = xz1 + xz2;
          b[j + s * (i + m2)] = wy1 - wy2;
          b[j + s * (i + m3)] = xz1 - xz2;
        }
        zi1 = zi1 * zeta;
        zi2 = zi1 * zi1;
        zi3 = zi1 * zi2;
      }
    }
    swap(a, b);
  }
  auto inv_n = modint<mod>(n).pow(mod - 2);
  for(int i = 0;i < n;i++) a[i] *= inv_n;
  return a;
}
