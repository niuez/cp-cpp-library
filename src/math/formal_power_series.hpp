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

#include <tuple>
#include "garner.hpp"

template<i64 M, i64... NTTis>
struct fps_multiply_arb {
  using fps_type = std::vector<modint<M>>;
  using conv_type = std::tuple<std::vector<modint<NTT_PRIMES[NTTis][0]>>...>;
  const static std::size_t tsize = std::tuple_size<conv_type>::value;

  template<i64 M2, i64 primitive>
  static std::vector<modint<M2>> dft_m2(const fps_type& arr) {
    std::vector<modint<M2>> res(arr.size());
    for(std::size_t i = 0; i < arr.size(); i++)
      res[i] = modint<M2>(arr[i].value());
    return number_theoretic_transform<M2, primitive>(std::move(res));
  }
  template<i64 M2, i64 primitive>
  static std::vector<modint<M2>> idft_m2(std::vector<modint<M2>> arr) {
    return inverse_number_theoretic_transform<M2, primitive>(std::move(arr));
  }
  template<std::size_t... I>
  static fps_type idft_func(std::index_sequence<I...>, conv_type arr) {
    arr = std::make_tuple(idft_m2<NTT_PRIMES[NTTis][0], NTT_PRIMES[NTTis][1]>(std::get<I>(arr))...);
    std::size_t len = std::get<0>(arr).size();
    static std::vector<i64> primes = { NTT_PRIMES[NTTis][0]... };
    fps_type res(len);
    for(std::size_t i = 0; i < len; i++) {
      std::vector<i64> x = { std::get<I>(arr)[i].value()... };
      res[i] = modint<M>(garner(x, primes, M));
    }
    return std::move(res);
  }
  template<i64 M2>
  static char mult_m2(std::vector<modint<M2>>& a, const std::vector<modint<M2>>& b) {
    for(int i = 0;i < a.size();i++) a[i] *= b[i];
    return 0;
  }
  template<std::size_t... I>
  static void mult_func(std::index_sequence<I...>, conv_type& a, const conv_type& b) {
    auto res = std::make_tuple(mult_m2<NTT_PRIMES[NTTis][0]>(std::get<I>(a), std::get<I>(b))...);
  }
  static conv_type dft(fps_type arr) {
    return std::make_tuple(dft_m2<NTT_PRIMES[NTTis][0], NTT_PRIMES[NTTis][1]>(arr)...);
  }
  static fps_type idft(conv_type arr) {
    return idft_func(std::make_index_sequence<tsize>(), std::move(arr));
  }
  static conv_type multiply(conv_type a, const conv_type& b) {
    mult_func(std::make_index_sequence<tsize>(), a, b);
    return a;
  }
};

template<class T, class fps_multiply>
struct FPS {
  std::vector<T> coef;

  static std::size_t bound_pow2(std::size_t sz) {
    return 1ll << (__lg(sz - 1) + 1);
  }
  
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

