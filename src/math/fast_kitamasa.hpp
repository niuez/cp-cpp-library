#include "formal_power_series.hpp"
#include <vector>
using i64 = long long;

template<class F, class FM>
std::vector<F> fast_kitamasa(std::vector<F> c, i64 n) {
  using fps = FPS<F, FM>;
  i64 k = c.size() - 1;
  fps cf(std::move(c));
  fps ic = cf.inv().pre(k - 1);
  
  i64 c_len = bound_pow2(k - 1 + cf.size() - 1);
  i64 ic_len = bound_pow2(k - 1 + ic.size() - 1);
  auto dc = FM::dft(cf.pre(c_len).coef);
  auto dic = FM::dft(ic.pre(ic_len).coef);

  i64 b_len = bound_pow2(k);
  std::vector<F> b(b_len, F(0));
  b[k - 1] = F(1);
  i64 m = bound_pow2(n);
  while(m) {
    b.resize(b_len * 2, F(0));
    auto bt = FM::dft(std::move(b));
    bt = FM::self_multiply(std::move(bt));
    fps beta(FM::idft(std::move(bt)));
    
    //  let q = (beta.clone().pre(k - 1) * ic.clone()).pre(k - 1);

    auto dbeta = FM::dft(beta.pre(k - 1).pre(ic_len).coef);
    auto q = FM::idft(FM::multiply(std::move(dbeta), dic));
    q.resize(c_len);
    for(int i = k - 1; i < ic_len; i++) q[i] = F(0);

    //beta -= cf * q;
    fps cfq(FM::idft(FM::multiply(FM::dft(std::move(q)), dc)));
    cfq.coef.resize(k - 1 + cf.size() - 1);
    beta -= cfq;

    b.assign(b_len * 2, F(0));
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
  return b;
}
