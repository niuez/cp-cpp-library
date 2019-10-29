#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct modint {
  i64 a;
  static i64& M() {
    static i64 mod_ = 2999;
    return mod_;
  }
  modint(const i64 x = 0) noexcept: a((x % M() + M()) % M()) {}
  i64 value() const noexcept { return a; }
  modint& operator=(const i64 r) {
    a = (r % M() + M()) % M();
    return *this;
  }
  modint& operator+=(const modint r) noexcept {
    a += r.a;
    if(a >= M()) a -= M();
    return *this;
  }
  modint& operator-=(const modint r) noexcept {
    a -= r.a;
    if(a < 0) a += M();
    return *this;
  }
  modint& operator*=(const modint r) noexcept {
    a = a * r.a % M();
    return *this;
  }
  modint& operator/=(modint r) noexcept {
    i64 ex = M() - 2;
    while(ex) {
      if(ex & 1) {
        *this *= r;
      }
      r *= r;
      ex >>= 1;
    }
    return *this;
  }

  modint operator+(const modint r) const {
    return modint(*this) += r;
  }
  modint operator-(const modint r) const {
    return modint(*this) -= r;
  }
  modint operator*(const modint r) const {
    return modint(*this) *= r;
  }
  modint operator/(const modint r) const {
    return modint(*this) /= r;
  }
};
