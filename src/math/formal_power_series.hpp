#include <vector>
using i64 = long long;

std::size_t bound_pow2(std::size_t sz) {
  return 1ll << (__lg(sz - 1) + 1);
}

template<class T, class fps_multiply>
struct FPS {
  std::vector<T> coef;

  
  FPS(const std::vector<T>& arr): coef(arr) {}
  size_t size() const { return coef.size(); }
  void bound_resize() {
    this->coef.resize(bound_pow2(this->size()));
  }
  T operator[](int i) const {
    if(i < coef.size()) return coef[i];
    else return T();
  }
  T & operator[](int i) { return coef[i]; }
  FPS pre(int n) const {
    std::vector<T> nex(n);
    for(int i = 0;i < coef.size() && i < n; i++) nex[i] = coef[i];
    return FPS(nex);
  }
  
  // F(0) must not be 0
  FPS inv() const {
    FPS g = FPS(std::vector<T>{ T(1) / (*this)[0] });
    this->bound_resize();
    i64 n = this->size();
    for(int i = 1;i < n;i <<= 1) {
      g = g.pre(i << 1);
      auto gdft = fps_multiply::dft(g.coef);
      auto e = fps_multiply::idft(
          fps_multiply::multiply(
            fps_multiply::dft(this->pre(i << 1).coef), gdft
            )
          );
      for(int j = 0;j < i;j++) {
        e[j] = T(0);
        e[j + i] = e[j + i] * T(-1);
      }
      auto f = fps_multiply::idft(
          fps_multiply::multiply(
            fps_multiply::dft(e), std::move(gdft)
            )
          );
      for(int j = 0;j < i;j++) {
        f[j] = g[j];
      }
      g.coef = std::move(f);
    }
    return g.pre(n);
  }

  FPS diff() const {
    FPS res(std::vector<T>(this->size() - 1, T(0)));
    for(i64 i = 1;i < this->size();i++) res[i - 1] = coef[i] * T(i);
    return res;
  }

  FPS integral() const {
    FPS res(std::vector<T>(this->size() + 1, T(0)));
    for(i64 i = 0;i < this->size();i++) res[i + 1] = coef[i] / T(i + 1);
    return res;
  }

  // F(0) must be 0
  FPS log() const {
    return (this->diff() * this->inv()).integral().pre(this->size());
  }

  FPS exp() const {
    FPS f(std::vector<T>{ T(1) });
    FPS g = *this;
    g.bound_resize();
    g[0] += T(1);
    for(i64 i = 1;i < size();i <<= 1 ) {
      f = (f * (g.pre(i << 1) - f.pre(i << 1).log())).pre(i << 1);
    }
    return f;
  }
  
  FPS operator+(const FPS& rhs) {
    i64 n = std::max(this->size(), rhs.size());
    std::vector<T> ans(n);
    for(int i = 0;i < n;i++) ans[i] = (*this)[i] + rhs[i];
    return FPS(ans);
  }
  FPS operator-(const FPS& rhs) {
    i64 n = std::max(this->size(), rhs.size());
    std::vector<T> ans(n);
    for(int i = 0;i < n;i++) ans[i] = (*this)[i] - rhs[i];
    return FPS(ans);
  }
  FPS operator*(const FPS& rhs) {
    i64 m = this->size() + rhs.size() - 1;
    i64 n = bound_pow2(m);
    auto res = fps_multiply::idft(
        fps_multiply::multiply(
          fps_multiply::dft(this->pre(n).coef), fps_multiply::dft(rhs.pre(n).coef)
          )
        );
    res.resize(m);
    return res;
  }

};

