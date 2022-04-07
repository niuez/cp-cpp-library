#include <iostream>
using i64 = long long;
template<i64 M> struct modint { i64 a;
  constexpr modint(const i64 x = 0): a((x%M+M)%M){}
  constexpr i64 value() const { return a; }
  constexpr modint inv() const { return this->pow(M-2); }
  constexpr modint pow(i64 r) const {
    modint ans(1); modint aa = *this;
    while(r) { if(r & 1) ans *= aa; aa *= aa; r >>= 1; }
    return ans;
  }
  constexpr bool operator==(const modint& r) const { return a == r.a; }
  constexpr bool operator!=(const modint& r) const { return a != r.a; }
  constexpr modint& operator=(const i64 r) { a = (r % M + M) % M; return *this; }
  constexpr modint& operator+=(const modint r) { a += r.a; if(a >= M) a -= M; return *this; }
  constexpr modint& operator-=(const modint r) { a -= r.a; if(a < 0) a += M; return *this; }
  constexpr modint& operator*=(const modint r) { a = a * r.a % M; return *this; }
  constexpr modint& operator/=(const modint r) { (*this) *= r.inv(); return *this; }
  constexpr modint operator+(const modint r) const { return modint(*this) += r; }
  constexpr modint operator-(const modint r) const { return modint(*this) -= r; }
  constexpr modint operator-() const { return modint(0) - modint(*this); }
  constexpr modint operator*(const modint r) const { return modint(*this) *= r; }
  constexpr modint operator/(const modint r) const { return modint(*this) /= r; }
  constexpr bool operator!=(const modint r) const { return this->value() != r.value(); }
};

template<const i64 M> std::ostream& operator<<(std::ostream& os, const modint<M>& m) { os << m.value(); return os; }


