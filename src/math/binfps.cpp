#include <array>
#include <cstdint>
#include <cassert>


#include <iostream>
#include <bitset>

template<const std::size_t N>
struct binfps {
  using size_type = std::size_t;
  using bit_type = std::uint_fast64_t;

  const static size_type lg = 6;
  const static size_type lgmask = (bit_type(1) << lg) - 1;
  const static size_type _w_len = (N + (bit_type(1) << lg) - 1) >> lg;
  std::array<bit_type, _w_len> w;
  size_type len;

  binfps(): len(0) {
    for(size_type i = 0; i < _w_len; i++) {
      w[i] = 0;
    }
  }

  binfps(const std::bitset<N>& b) {
    for(size_type i = 0; i < _w_len; i++) {
      w[i] = 0;
    }
    for(size_type i = 0; i < N; i++) {
      if(b[i]) set(i);
    }
    recalc();
  }

  int size() const { return this->len; }
  void recalc() {
    len = 0;
    for(size_type i = w.size(); i --> 0; ) {
      if(w[i]) {
        for(size_type j = (1 << lg); j --> 0;) {
          if(w[i] >> j) {
            len = j + 1 + (i << lg);
            return;
          }
        }
      }
    }
  }

  void set(size_type i) { w[i >> lg] |= (bit_type(1) << (i & lgmask)); }
  void unset(size_type i) { w[i >> lg] &= ~(bit_type(1) << (i & lgmask)); }

  bool any() const {
    for(size_type i = 0; i < _w_len; i++) {
      if(w[i]) {
        return true;
      }
    }
    return false;
  }

  bool operator[](size_type i) const {
    return (w[i >> lg] >> (i & lgmask)) & 1;
  }

  binfps& operator^=(const binfps& b) {
    for(size_type i = w.size(); i --> 0;) {
      w[i] ^= b.w[i];
    }
    return *this;
  }
  binfps& operator&=(const binfps& b) {
    for(size_type i = w.size(); i --> 0;) {
      w[i] &= b.w[i];
    }
    return *this;
  }
  binfps& operator|=(const binfps& b) {
    for(size_type i = w.size(); i --> 0;) {
      w[i] &= b.w[i];
    }
    return *this;
  }


  binfps& operator<<=(size_type x) {
    std::array<bit_type, _w_len> next;
    for(size_type i = 0; i < _w_len; i++) {
      next[i] = 0;
    }
    size_type off = x >> lg;
    size_type m = x & lgmask;
    bit_type dwmask = ((bit_type)(1) << (64 - m)) - 1;
    if(m == 0) {
      dwmask = ~(bit_type)(0);
    }
    bit_type upmask = ~dwmask;
    // up
    for(size_type i = 0; i + off + 1 < _w_len; i++) {
      next[i + off + 1] |= (w[i] & upmask) >> (64 - m);
    }
    // down
    for(size_type i = 0; i + off < _w_len; i++) {
      next[i + off] |= (w[i] & dwmask) << m;
    }

    w = std::move(next);
    len = std::min(N, len + x);
    return (*this);
  }

  binfps& operator>>=(size_type x) {
    std::array<bit_type, _w_len> next;
    for(size_type i = 0; i < _w_len; i++) {
      next[i] = 0;
    }
    bit_type off = x >> lg;
    bit_type m = x & lgmask;
    bit_type dwmask = (bit_type(1) << m) - 1;
    if(m == 0) {
      dwmask = 0;
    }
    bit_type upmask = ~dwmask;
    // down
    for(size_type i = 0; i + off + 1 < _w_len; i++) {
      next[i] |= (w[i + off + 1] & dwmask) << (64 - m);
    }
    // up
    for(size_type i = 0; i + off < _w_len; i++) {
      next[i] |= (w[i + off] & upmask) >> m;
    }
    w = std::move(next);
    if(len < x) {
      len = 0;
    }
    else {
      len = len - x;
    }
    return (*this);
  }

  binfps operator^(const binfps& b) const { return binfps(*this) ^= b; }
  binfps operator&(const binfps& b) const { return binfps(*this) &= b; }
  binfps operator|(const binfps& b) const { return binfps(*this) |= b; }
  binfps operator<<(const size_type x) const { return binfps(*this) <<= x; }
  binfps operator>>(const size_type x) const { return binfps(*this) >>= x; }
  binfps operator~() {
    binfps a = *this;
    for(size_type i = w.size(); i --> 0;) {
      a.w[i] = ~w[i];
    }
    return a;
  }

  bool operator<(const binfps& b) const {
    bool OK = false;
    for(size_type i = _w_len; i --> 0; ) {
      if(w[i] != b.w[i]) {
        if(w[i] < b.w[i]) {
          OK = true;
        }
        break;
      }
    }
    return OK;
  }
  bool operator<=(const binfps& b) const {
    bool OK = true;
    for(size_type i = _w_len; i --> 0; ) {
      if(w[i] != b.w[i]) {
        if(w[i] > b.w[i]) {
          OK = false;
        }
        break;
      }
    }
    return OK;
  }

  static binfps mod(binfps a, const binfps& b) {
    assert(b.size() > 0);
    for(int i = (int)a.size() - (int)b.size() + 1; i --> 0;) {
      if(a[i + b.size() - 1]) {
        a ^= (b << i);
      }
    }
    a.recalc();
    return a;
  }

  static binfps div(binfps a, const binfps& b) {
    assert(b.size() > 0);
    binfps d;
    for(int i = (int)a.size() - (int)b.size() + 1; i --> 0;) {
      if(a[i + b.size() - 1]) {
        a ^= (b << i);

        binfps e;
        e.set(i);
        d ^= e;
      }
    }
    d.recalc();
    return d;
  }

  static binfps gcd(binfps a, binfps b) {
    while(b.any()) {
      auto m = mod(a, b);
      a = std::move(b);
      b = std::move(m);
    }
    return a;
  }

  void dump() const {
    for(size_type i = _w_len; i --> 0; ) {
      std::cerr << std::bitset<64>(w[i]) << "|" << std::endl;
    }
    std::cerr << std::endl;
  }
};


const int BN = 5000;
using bits = binfps<BN>;

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define all(x) x.begin(),x.end()

template<class T>
static inline std::vector<T> ndvec(size_t&& n, T val) noexcept {
  return std::vector<T>(n, std::forward<T>(val));
}

template<class... Tail>
static inline auto ndvec(size_t&& n, Tail&&... tail) noexcept {
  return std::vector<decltype(ndvec(std::forward<Tail>(tail)...))>(n, ndvec(std::forward<Tail>(tail)...));
}
 
int main() {
  i64 N;
  cin >> N;
  std::bitset<BN> binp;
  cin >> binp;
  bits X(binp);
  X.recalc();
 
  vector<bits> A(N);
  rep(i,0,N) {
    cin >> binp;
    A[i] = bits(binp);
    A[i].recalc();
  }
  auto G = A[0];
  for(i64 i = 1; i < N; i++) {
    G = bits::gcd(G, A[i]);
  }
  i64 ans = 0;
  const i64 MOD = 998244353;
  if(X.size() - G.size() + 1 >= 0) {
    vector<i64> Bs(X.size() - G.size() + 1, 1);
    for(i64 i = 1; i < X.size() - G.size() + 1; i++) {
      Bs[i] = (Bs[i - 1] * 2) % MOD;
    } 
    bits now;
    for(i64 i = X.size() - G.size() + 1; i --> 0;) {
      if(X[i + G.size() - 1]) {
        ans = (ans + Bs[i]) % MOD;
      }
      if(now[i + G.size() - 1] != X[i + G.size() - 1]) {
        now ^= (G << i);
      }
    }
    if(now <= X) ans = (ans + 1) % MOD;
  }
  cout << ans << endl;
}
