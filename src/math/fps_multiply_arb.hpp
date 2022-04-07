#include "modint.hpp"
#include "convolution/number_theoretic_transform.hpp"
#include "garner.hpp"
#include <vector>
#include <tuple>

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
  static conv_type self_multiply(conv_type a) {
    mult_func(std::make_index_sequence<tsize>(), a, a);
    return a;
  }
};

