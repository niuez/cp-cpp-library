
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

template<const i64 mod, const i64 primitive>
vector<modint<mod>> number_theoretic_transform4(vector<modint<mod>> a) {
  i64 n = a.size();
  vector<modint<mod>> b(a.size());
  auto unit_i = modint<mod>(primitive).pow((mod - 1) / 4);
  for(i64 s = 1; s < n; s <<= 1) {
    i64 m = n / 2 / s;
    if(m == 1) {
      for(i64 j = 0;j < s;j++) {
        auto x = a[j + 0];
        auto y = a[j + s];
        b[j + 0] = x + y;
        b[j + s] = x - y;
      }
    }
    else {
      modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (m << 1));
      modint<mod> zi1 = 1;
      modint<mod> zi2 = 1;
      modint<mod> zi3 = 1;
      i64 m1 = m / 2;
      i64 m2 = m;
      i64 m3 = m1 + m2;
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
          b[j + s * (4 * i + 1)] = (wy2 - xz2) * zi1;
          b[j + s * (4 * i + 2)] = (wy1 - xz1) * zi2;
          b[j + s * (4 * i + 3)] = (wy2 + xz2) * zi3;
        }
        zi1 = zi1 * zeta;
        zi2 = zi1 * zi1;
        zi3 = zi1 * zi2;
      }
      s <<= 1;
    }
    swap(a, b);
  }
  return a;
}

template<const i64 mod, const i64 primitive>
vector<modint<mod>> inverse_number_theoretic_transform4(vector<modint<mod>> a) {
  i64 n = a.size();
  vector<modint<mod>> b(a.size());
  auto unit_i = modint<mod>(primitive).pow((mod - 1) / 4).pow(mod - 2);
  i64 s = n;
  if(__builtin_ctzll(n) & 1) {
    s >>= 1;
    for(i64 j = 0;j < s;j++) {
      auto x = a[j + 0];
      auto y = a[j + s];
      b[j + 0] = x + y;
      b[j + s] = x - y;
    }
    swap(a, b);
  }
  for(; s >>= 2, s >= 1;) {
    i64 m = n / 2 / s;
    {
      modint<mod> zeta = modint<mod>(primitive).pow((mod - 1) / (m << 1)).pow(mod - 2);
      modint<mod> zi1 = 1;
      modint<mod> zi2 = 1;
      modint<mod> zi3 = 1;
      i64 m1 = m / 2;
      i64 m2 = m;
      i64 m3 = m1 + m2;
      for(i64 i = 0;i < m1;i++) {
        for(i64 j = 0;j < s;j++) {
          auto w = a[j + s * (4 * i + 0)];
          auto x = a[j + s * (4 * i + 1)] * zi1;
          auto y = a[j + s * (4 * i + 2)] * zi2;
          auto z = a[j + s * (4 * i + 3)] * zi3;
          auto wy1 = w + y;
          auto wy2 = x + z;
          auto xz1 = w - y;
          auto xz2 = (z - x) * unit_i;
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

#include <cstdio>

namespace niu {
  char cur;
  struct FIN {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline char next() { return cur = getc_unlocked(stdin); }
    inline char peek() { return cur; }
    inline void skip() { while(is_blank(next())){} }
#define intin(inttype)  \
    FIN& operator>>(inttype& n) { \
      bool sign = 0; \
      n = 0; \
      skip(); \
      while(!is_blank(peek())) { \
        if(peek() == '-') sign = 1; \
        else n = (n << 1) + (n << 3) + (peek() & 0b1111); \
        next(); \
      } \
      if(sign) n = -n; \
      return *this; \
    }
intin(int)
intin(long long)
  } fin;

  char tmp[128];
  struct FOUT {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline void push(char c) { putc_unlocked(c, stdout); }
    FOUT& operator<<(char c) { push(c); return *this; }
    FOUT& operator<<(const char* s) { while(*s) push(*s++); return *this; }
#define intout(inttype) \
    FOUT& operator<<(inttype n) { \
      if(n) { \
        char* p = tmp + 127; bool neg = 0; \
        if(n < 0) neg = 1, n = -n; \
        while(n) *--p = (n % 10) | 0b00110000, n /= 10; \
        if(neg) *--p = '-'; \
        return (*this) << p; \
      } \
      else { \
        push('0'); \
        return *this; \
      } \
    }
intout(int)
intout(long long)
  } fout;
}
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)

int main() {
  using niu::fin;
  using niu::fout;

  i64 N, M;
  cin >> N >> M;
  i64 X = 1;
  i64 Y = N + M - 1;
  while(X < Y) X <<= 1;
  const i64 MOD = 998244353;
  using fp = modint<MOD>;
  std::vector<fp> A(X), B(X);
  rep(i, 0, N) fin >> A[i].a;
  rep(i, 0, M) fin >> B[i].a;
  A = number_theoretic_transform4<MOD, 3>(std::move(A));
  B = number_theoretic_transform4<MOD, 3>(std::move(B));
  rep(i,0,X) {
    A[i] *= B[i];
  }
  A = inverse_number_theoretic_transform4<MOD, 3>(std::move(A));
  rep(i,0,N + M - 2) {
    fout << A[i].value() << " ";
  }
  fout << A[N + M - 2].value() << "\n";
}
