#include "formal_power_series.hpp"
#include <vector>
using i64 = long long;

template<class F, class FM>
std::vector<F> fast_kitamasa(std::vector<F> c, i64 n) {
  using fps = FPS<FM>;
  i64 k = c.size() - 1;
  fps cf(std::move(c));
  fps ic = cf.inv(k - 1);
  
  i64 c_len = bound_pow2(k - 1 + cf.size() - 1);
  i64 ic_len = bound_pow2(k - 1 + ic.size() - 1);
  auto dc = cf.resized(c_len).dft(c_len);
  auto dic = std::move(ic).dft(ic_len);

  i64 b_len = bound_pow2(k);
  fps b(std::vector<F>(b_len, F(0)));
  b[k - 1] = F(1);
  i64 m = bound_pow2(n);
  while(m) {
    auto bt = std::move(b).dft(b_len * 2);
    bt *= bt;
    auto beta = std::move(bt).idft();
    
    auto dbeta = beta.resized(k - 1).dft(ic_len);
    auto q = std::move(dbeta *= dic).idft(c_len);
    for(int i = k - 1; i < c_len; i++) q[i] = F(0);
    fps cfq = std::move(std::move(q).dft(c_len) *= dc).idft(k - 1 + cf.size() - 1);
    beta -= cfq;

    b = fps(std::vector<F>(b_len * 2));
    for(int i = k - 1; i < 2 * k - 1; i++) {
      b[i - (k - 1)] = beta[i];
    }
    if(m & n) {
      F freq = b[0];
      for(int i = 0; i < k - 1; i++) {
        b[i] = b[i + 1] + freq * cf[i + 1];
      }
      b[k - 1] = freq * cf[k];
    }
    m >>= 1;
  }
  b.resize(k);
  return std::move(b.coef);
}
