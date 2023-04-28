#include <vector>
using i64 = long long;

std::size_t bound_pow2(std::size_t sz) {
  return 1ll << (std::__lg(sz - 1) + 1);
}

template<class fps_multiply>
struct FPS {
  struct DFT {
    using C = typename fps_multiply::conv_type;
    std::vector<C> coef;
    DFT(std::vector<C> c): coef(std::move(c)) {}
    DFT clone() const {
      return DFT(coef);
    }
    std::size_t size() const {
      return coef.size();
    }
    C& operator[](int i) {
      return coef[i];
    }
    const C& operator[](int i) const {
      return coef[i];
    }
    DFT& operator+=(const DFT& r) {
      assert(coef.size() == r.size());
      for(int i = 0;i < coef.size(); i++) {
        coef[i] += r[i];
      }
      return (*this);
    }
    DFT& operator-=(const DFT& r) {
      assert(coef.size() == r.size());
      for(int i = 0;i < coef.size(); i++) {
        coef[i] -= r[i];
      }
      return (*this);
    }
    DFT& operator*=(const DFT& r) {
      assert(coef.size() == r.size());
      for(int i = 0;i < coef.size(); i++) {
        coef[i] *= r[i];
      }
      return (*this);
    }
    DFT& operator*=(const C& t) {
      for(int i = 0; i < coef.size(); i++) {
        coef[i] *= t;
      }
      return (*this);
    }
    DFT operator+(const DFT& r) const { return DFT(*this) += r; }
    DFT operator-(const DFT& r) const { return DFT(*this) -= r; }
    DFT operator*(const DFT& r) const { return DFT(*this) *= r; }
    DFT operator*(const C& r) const { return DFT(*this) *= r; }
    FPS idft(int n = -1) && {
      auto res = fps_multiply::idft(std::move(coef));
      if(n > 0) {
        res.resize(n);
      }
      return FPS(std::move(res));
    }
  };
  using T = typename fps_multiply::fps_type;
  std::vector<T> coef;
  FPS(std::vector<T> f): coef(std::move(f)) {}
  void resize(int n) { coef.resize(n, T(0)); }
  FPS resized(int n) const {
    std::vector<T> res(n);
    for(int i = 0;i < n && i < coef.size();i++) res[i] = coef[i];
    return FPS(std::move(res));
  }
  FPS clone() const {
    return FPS(coef);
  }
  std::size_t size() const {
    return coef.size();
  }
  T& operator[](int i) {
    return coef[i];
  }
  const T& operator[](int i) const {
    return coef[i];
  }
  FPS& operator+=(const FPS& r) {
    if(coef.size() < r.size()) coef.resize(r.size());
    for(int i = 0;i < coef.size() && i < r.size(); i++) {
      coef[i] += r[i];
    }
    return (*this);
  }
  FPS& operator-=(const FPS& r) {
    if(coef.size() < r.size()) coef.resize(r.size());
    for(int i = 0;i < coef.size() && i < r.size(); i++) {
      coef[i] -= r[i];
    }
    return (*this);
  }
  FPS& operator*=(const T& t) {
    for(int i = 0; i < coef.size(); i++) {
      coef[i] *= t;
    }
  }
  FPS operator+(const FPS& r) const { return FPS(*this) += r; }
  FPS operator-(const FPS& r) const { return FPS(*this) -= r; }
  FPS operator*(const T& r) const { return FPS(*this) *= r; }
  DFT dft(int n) && {
    assert(!(n & (n - 1)));
    coef.resize(n);
    return DFT(fps_multiply::dft(std::move(coef)));
  }

  FPS inv(int n) const {
    FPS g = FPS(std::vector<T>{ T(1) / (*this)[0] });
    for(int i = 1;i < n;i <<= 1) {
      DFT gdft = g.resized(i << 1).dft(i << 1);
      FPS e = (gdft * this->resized(i << 1).dft(i << 1)).idft();
      for(int j = 0;j < i;j++) {
        e[j] = T(0);
        e[j + i] = e[j + i] * T(-1);
      }
      FPS f = std::move(gdft *= std::move(e).dft(i << 1)).idft();
      for(int j = 0;j < i;j++) {
        f[j] = g[j];
      }
      g = std::move(f);
    }
    g.resize(n);
    return g;
  }

  FPS diff(int n) const {
    std::vector<T> res(n);
    for(int i = 0; i + 1 < this->size() && i < n; i++) {
      res[i] = coef[i + 1] * T(i + 1);
    }
    return FPS(std::move(res));
  }

  FPS integral(int n) const {
    std::vector<T> res(n);
    int m = std::min(n, int(coef.size() + 1));
    res[0] = T(1);
    for(int i = 1; i < m; i++) {
      res[i] = res[i - 1] * T(i);
    }
    T finv = T(1) / res[m - 1];
    for(int i = m; i --> 1;) {
      res[i] = coef[i - 1] * finv * res[i - 1];
      finv *= T(i);
    }
    res[0] = T(0);
    return FPS(std::move(res));
  }

  // F(0) must not be 0
  FPS log(int n) const {
    FPS in = this->inv(n);
    FPS di = this->diff(n);
    int m = bound_pow2(n);
    return (std::move(di).dft(m * 2) * std::move(in).dft(m * 2)).idft().integral(n);
  }

  FPS exp(int n) const {
    FPS f(std::vector<T>{ T(1) });
    for(i64 i = 1;i < n;i <<= 1 ) {
      FPS flog = f.log(i << 1);
      for(int j = 0; j < (i << 1); j++) {
        flog[j] = (j < coef.size() ? coef[j] - flog[j] : -flog[j]);
      }
      flog[0] += T(1);
      f = (std::move(f).dft(i << 2) * std::move(flog).dft(i << 2)).idft(i << 1);
    }
    f.resize(n);
    return f;
  }
};
