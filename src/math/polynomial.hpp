#include <vector>
#include <array>
#include <complex>
#include <cassert>
#include <set>
#include <iostream>
using i64 = long long;

namespace fft {

  const double pi = std::acos(-1);
  
  std::vector<std::complex<double>> fast_fourier_transform(std::vector<std::complex<double>> a, bool inv = false) {
    i64 n = a.size();
    std::vector<std::complex<double>> tmp(n);
    std::vector<i64> ai(n);
    std::vector<i64> ti(n);
    for(i64 i = 0; i < n; i++) ai[i] = i;

    for(i64 s = n >> 1; s >= 1; s >>= 1) {
      swap(tmp, a);
      swap(ai, ti);
      std::complex<double> zeta = std::polar(1.0, (inv ? -1: 1) * 2 * pi / (double)(s << 1));
      std::complex<double> z_i = 1.0;
      i64 ev = 0;
      i64 od = 1;
      for(i64 i = 0; i < n; i++) {
        if(i & s) {
          a[i] = (tmp[i - s] - tmp[i]) * z_i;
          ai[i] = ti[od];
          od += 2;
          z_i *= zeta;
        }
        else {
          a[i] = tmp[i] + tmp[i + s];
          ai[i] = ti[ev];
          ev += 2;
          z_i = 1.0;
        }
      }
    }
    swap(tmp, a);

    for(i64 i = 0; i < n; i++) a[ai[i]] = tmp[i];
    if(inv) for(i64 i = 0; i < n; i++) { a[i] /= (double)(n); }
    return a;
  }
  
  template<class T>
  std::vector<T> multiply(const std::vector<T>& a, const std::vector<T>& b) {
    i64 deg = a.size() + b.size() - 1;
    i64 n = 1;
    while(n < deg) n <<= 1;
    std::vector<std::complex<double>> A(n), B(n);
    for(i64 i = 0; i < a.size(); i++) A[i] = a[i];
    for(i64 i = 0; i < b.size(); i++) B[i] = b[i];
    std::vector<std::complex<double>> Af = fast_fourier_transform(A);
    std::vector<std::complex<double>> Bf = fast_fourier_transform(B);
    for(i64 i = 0; i < n; i++) Af[i] *= Bf[i];
    std::vector<std::complex<double>> C = fast_fourier_transform(Af, true);
    std::vector<T> c(deg);
    for(i64 i = 0; i < deg; i++) c[i] = std::round(C[i].real());
    return c;
  }
}

template<class T>
class polynomial {
  std::vector<T> a;
  i64 N;

public:

  polynomial(const i64 deg): a(deg + 1), N(deg) {}
  polynomial(const std::vector<T> arr): a(arr), N((i64)arr.size() - 1) {}

  i64 degree() const { return N; }

  const T& operator[](const i64 i) const { return a.at(i); }
  T& operator[](const i64 i) { return a.at(i); }

  T& operator()(const T x) {
    T ans = 0;
    T xx = 1;
    for(int i = 0;i <= N;i++) {
      ans += a[i] * xx;
      xx *= x;
    }
    return ans;
  };

  polynomial& operator+=(const polynomial& po) {
    if(this->degree() < po.degree()) a.resize(po.degree() + 1);
    assert(po.degree() == this->degree());
    for(int i = 0;i <= N;i++) {
      a[i] += po[i];
    }
    return *this;
  }
  polynomial& operator-=(const polynomial& po) {
    if(this->degree() < po.degree()) a.resize(po.degree() + 1);
    for(int i = 0;i <= N;i++) {
      a[i] -= po[i];
    }
    return *this;
  }
  polynomial& operator*=(const polynomial& po) {
    a = fft::multiply(a, po.a);
    N = a.size() - 1;
    return *this;
  }
  polynomial& operator*=(const T& t) {
    for(int i = 0;i <= N;i++) {
      a[i] *= t;
    }
    return *this;
  }
  polynomial& operator/=(const T& t) {
    for(int i = 0;i <= N;i++) {
      a[i] /= t;
    }
    return *this;
  }

  polynomial operator+(const polynomial& po) {
    return polynomial(*this) += po;
  }
  polynomial operator-(const polynomial& po) {
    return polynomial(*this) -= po;
  }
  polynomial operator*(const polynomial& po) {
    return polynomial(*this) *= po;
  }
  polynomial operator*(const T& t) {
    return polynomial(*this) *= t;
  }
  polynomial operator/(const polynomial& po) {
    return polynomial(*this) /= po;
  }
  polynomial operator/(const T& t) {
    return polynomial(*this) /= t;
  }
  
  template<class U>
  friend std::ostream& operator<<(std::ostream& os, const polynomial<U>& p);
};

template<class U>
std::ostream& operator<<(std::ostream& os, const polynomial<U>& p) {
  for(int i = 0;i <= p.degree();i++) {
    os << p[i] << "x^" << i;
    if(i < p.degree()) os << " + ";
  }
  return os;
}

template<class T>
polynomial<T> lagrange_interpolation(std::vector<std::pair<T, T>> ps) {
  polynomial<T> ans((i64)ps.size() - 1);
  for(int i = 0;i < ps.size();i++) {
    T x = ps[i].first;
    T y = ps[i].second;
    polynomial<T> f(std::vector<T>{ 1 });
    T z = 1;
    for(int j = 0;j < ps.size();j++) {
      if(i == j) continue;
      f *= polynomial<T>(std::vector<T>{ -ps[j].first, 1 });
      z *= x - ps[j].first;
    }
    ans += f * y / z;
  }
  return ans;
}

template<class T>
polynomial<T> newton_interpolation(std::vector<std::pair<T, T>> ps) {
  std::vector<T> diff(ps.size());
  diff[0] = ps[0].second;
  std::vector<T> x(ps.size()), y(ps.size());
  for(int i = 0;i < ps.size(); i++) {
    x[i] = ps[i].first, y[i] = ps[i].second;
  }
  for(int i = 1;i < ps.size();i++) {
    std::vector<T> d(ps.size() - i);
    for(int j = 0;j + i < ps.size(); j++) {
      d[j] = (y[j + 1] - y[j]) / (x[j + i] - x[j]);
    }
    diff[i] = d[0];
    swap(y, d);
  }

  std::vector<T> ans(ps.size());

  std::vector<T> c(ps.size());
  c[0] = 1;

  for(int i = 0;i < ps.size();i++) {
    for(int j = 0;j <= ps.size();j++) {
      ans[j] += diff[i] * c[j];
    }
    if(i + 1 == ps.size()) break;

    std::vector<T> next(ps.size() + 1);
    for(int j = 0;j < ps.size();j++) {
      next[j + 1] = c[j];
    }
    for(int j = 0;j < ps.size();j++) {
      next[j] -= c[j] * ps[i].first;
    }
    swap(c, next);
  }
  return polynomial<T>(ans);
}


int main() {
  polynomial<i64> p1(std::vector<i64>{ 1, 2, 3 });
  polynomial<i64> p2(std::vector<i64>{ 2, 3, 4 });

  std::cout << p1 << std::endl;
  std::cout << p2 << std::endl;
  std::cout << p1 + p2 << std::endl;
  std::cout << p1 - p2 << std::endl;
  std::cout << p1 * p2 << std::endl;

  std::cout << lagrange_interpolation(std::vector<std::pair<double, double>>{ {-1.0, 0}, {1.0, 0}, { 0, 1.0 } }) << std::endl;
  std::cout << newton_interpolation(std::vector<std::pair<double, double>>{ {-1.0, 0}, {1.0, 0}, { 0, 1.0 } }) << std::endl;
}
