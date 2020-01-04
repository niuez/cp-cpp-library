#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template<i64 M>
struct modint {
  i64 a;
  constexpr modint(const i64 x = 0) noexcept: a((x % M + M) % M) {}
  constexpr i64 value() const noexcept { return a; }
  constexpr modint pow(i64 r) const noexcept {
    modint ans(1);
    modint aa = *this;
    while(r) {
      if(r & 1) {
        ans *= aa;
      }
      aa *= aa;
      r >>= 1;
    }
    return ans;
  }
  constexpr modint& operator+=(const modint r) noexcept {
    a += r.a;
    if(a >= M) a -= M;
    return *this;
  }
  constexpr modint& operator=(const i64 r) {
    a = (r % M + M) % M;
    return *this;
  }
  constexpr modint& operator-=(const modint r) noexcept {
    a -= r.a;
    if(a < 0) a += M;
    return *this;
  }
  constexpr modint& operator*=(const modint r) noexcept {
    a = a * r.a % M;
    return *this;
  }
  constexpr modint& operator/=(modint r) noexcept {
    i64 ex = M - 2;
    while(ex) {
      if(ex & 1) {
        *this *= r;
      }
      r *= r;
      ex >>= 1;
    }
    return *this;
  }

  constexpr modint operator+(const modint r) const {
    return modint(*this) += r;
  }
  constexpr modint operator-(const modint r) const {
    return modint(*this) -= r;
  }
  constexpr modint operator*(const modint r) const {
    return modint(*this) *= r;
  }
  constexpr modint operator/(const modint r) const {
    return modint(*this) /= r;
  }

  constexpr bool operator!=(const modint r) const {
    return this->value() != r.value();
  }

};

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
  vector<modint<mod>> b(a.size());
  for(i64 s = 1; s < n; s <<= 1) {
    i64 m = n / 2 / s;
    modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (m << 1));
    modint<mod> zi = 1;
    for(i64 i = 0;i < m;i++) {
      for(i64 j = 0;j < s;j++) {
        auto x = a[j + s * (i + 0)];
        auto y = a[j + s * (i + m)];
        b[j + s * (2 * i + 0)] = x + y;
        b[j + s * (2 * i + 1)] = (x - y) * zi;
      }
      zi = zi * zeta;
    }
    swap(a, b);
  }
  return a;
}

template<const i64 mod, const i64 primitive>
vector<modint<mod>> inverse_number_theoretic_transform(vector<modint<mod>> a) {
  i64 n = a.size();
  vector<modint<mod>> b(a.size());
  for(i64 s = n; s >>= 1, s >= 1;) {
    i64 m = n / 2 / s;
    modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (m << 1)).pow(mod - 2);
    modint<mod> zi = 1;
    for(i64 i = 0;i < m;i++) {
      for(i64 j = 0;j < s;j++) {
        auto x = a[j + s * (2 * i + 0)];
        auto y = a[j + s * (2 * i + 1)] * zi;
        b[j + s * (i + 0)] = x + y;
        b[j + s * (i + m)] = x - y;
      }
      zi = zi * zeta;
    }
    swap(a, b);
  }
  auto inv_n = modint<mod>(n).pow(mod - 2);
  for(int i = 0;i < n;i++) a[i] *= inv_n;
  return a;
}
