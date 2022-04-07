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
  static std::vector<conv_type> multiply(std::vector<conv_type> a, const std::vector<conv_type>& b) {
    for(int i = 0;i < a.size();i++) a[i] *= b[i];
    return a;
  }
  static std::vector<conv_type> self_multiply(std::vector<conv_type> a) {
    for(int i = 0;i < a.size();i++) a[i] *= a[i];
    return a;
  }
};

