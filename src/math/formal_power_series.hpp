#include "modint.hpp"
#include "convolution/number_theoretic_transform.hpp"

template<const i64 mod, const i64 primitive>
struct fps_ntt_multiply {
  using fps_type = modint<mod>;
  using conv_type = modint<mod>;
  static std::vector<conv_type> dft(std::vector<fps_type> arr) {
    return number_theoretic_transform<mod, primitive>(std::move(arr));
  }
  static std::vector<fps_type> idft(std::vector<conv_type> arr) {
    return inverse_number_theoretic_transform<mod, primitive>(std::move(arr));
  }
  static std::vector<conv_type> multiply(std::vector<conv_type> a, std::vector<conv_type> b) {
    for(int i = 0;i < a.size();i++) a[i] *= b[i];
    return a;
  }
};

#include <vector>

template<class T, class fps_multiply>
struct FPS {
  std::vector<T> coef;
  
  FPS(const std::vector<T>& arr): coef(arr) {
    i64 n = 1;
    i64 m = arr.size();
    while(n < m) n <<= 1;
    coef.resize(n);
  }
  size_t size() const { return coef.size(); }
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
    FPS res(vector<T>(this->size() - 1, T(0)));
    for(i64 i = 1;i < this->size();i++) res[i - 1] = coef[i] * T(i);
    return res;
  }

  FPS integral() const {
    FPS res(vector<T>(this->size() + 1, T(0)));
    for(i64 i = 0;i < this->size();i++) res[i + 1] = coef[i] / T(i + 1);
    return res;
  }

  // F(0) must be 0
  FPS log() const {
    return (this->diff() * this->inv()).integral().pre(this->size());
  }

  FPS exp() const {
    FPS f(vector<T>{ T(1) });
    FPS g = *this;
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
    i64 n = std::max(this->size(), rhs.size()) << 1;
    return fps_multiply::idft(
        fps_multiply::multiply(
          fps_multiply::dft(this->pre(n).coef), fps_multiply::dft(rhs.pre(n).coef)
          )
        );
  }

};

