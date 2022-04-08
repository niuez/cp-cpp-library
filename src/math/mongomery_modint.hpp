#include <iostream>

template<uint32_t M>
struct montgomery_modint {
  using Self = montgomery_modint<M>;
  using i32 = int32_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  static constexpr u32 get_r() {
    u32 res = M;
    for(int i = 0; i < 4; i++) res *= 2 - M * res;
    return res;
  }
  static constexpr u32 reduce(u64 a) {
    return (a + u64(u32(a) * u32(-r)) * M) >> 32;
  }

  static constexpr u32 r = get_r();
  static constexpr u32 n2 = -u64(M) % M;

  u32 a;

  constexpr montgomery_modint() : a(0) {}
  constexpr montgomery_modint(int64_t a) : a(reduce(u64(a % M + M) * n2)) {}

  constexpr u32 val() const {
    u32 res = reduce(a);
    return res >= M ? res - M : res;
  }
  constexpr Self pow(u64 r) const {
    Self ans(1); Self aa = *this;
    while(r) { if(r & 1) ans *= aa; aa *= aa; r >>= 1; }
    return ans;
  }
  constexpr Self inv() const { return this->pow(M - 2); }
  constexpr Self& operator+=(const Self& r) {
    if(i32(a += r.a - 2 * M) < 0) a += 2 * M;
    return *this;
  }
  constexpr Self& operator-=(const Self& r) {
    if(i32(a -= r.a) < 0) a += 2 * M;
    return *this;
  }
  constexpr Self& operator*=(const Self& r) {
    a = reduce(u64(a) * r.a);
    return *this;
  }
  constexpr Self& operator/=(const Self& r) {
    *this *= r.inv();
    return *this;
  }
  constexpr Self operator+(const Self r) const { return Self(*this) += r; }
  constexpr Self operator-(const Self r) const { return Self(*this) -= r; }
  constexpr Self operator-() const { return Self() - Self(*this); }
  constexpr Self operator*(const Self r) const { return Self(*this) *= r; }
  constexpr Self operator/(const Self r) const { return Self(*this) /= r; }
  constexpr bool operator==(const Self& r) const {
    return (a >= M ? a - M : a) == (r.a >= M ? r.a - M : r.a);
  }
  constexpr bool operator!=(const Self& r) const {
    return (a >= M ? a - M : a) == (r.a >= M ? r.a - M : r.a);
  }
};

template<uint32_t M>
std::ostream& operator<<(std::ostream& os, const montgomery_modint<M>& m) {
  return os << m.val();
}
template<uint32_t M>
std::istream& operator>>(std::istream& is, montgomery_modint<M>& m) {
  int64_t t;
  is >> t;
  m = montgomery_modint<M>(t);
  return is;
}

template<uint32_t mod>
using modint = montgomery_modint<mod>;
