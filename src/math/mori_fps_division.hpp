#include "formal_power_series.hpp"

template<class fps_multiply>
typename fps_multiply::fps_type mori_fps_division(FPS<fps_multiply> P, FPS<fps_multiply> Q, std::size_t N) {
  using T = typename fps_multiply::fps_type;
  i64 k = bound_pow2(std::max(P.size(), Q.size()));
  P.resize(k * 2);
  Q.resize(k * 2);
  while(N > 0) {
    auto Qm = Q;
    for(std::size_t i = 1; i < Qm.size(); i += 2) {
      Qm[i] *= T(-1);
    }
    //auto Pd = fps_multiply::dft(std::move(P.coef));
    //auto Qd = fps_multiply::dft(std::move(Q.coef));
    //auto Qmd = fps_multiply::dft(std::move(Qm.coef));
    auto Pd = std::move(P).dft(k * 2);
    auto Qd = std::move(Q).dft(k * 2);
    auto Qmd = std::move(Qm).dft(k * 2);
    //auto PQ = fps_multiply::idft(
    //    fps_multiply::multiply(std::move(Pd), Qmd)
    //    );
    auto PQ = std::move(Pd *= Qmd).idft();
    //auto QQ = fps_multiply::idft(
    //    fps_multiply::multiply(std::move(Qd), std::move(Qmd))
    //    );
    auto QQ = std::move(Qd *= Qmd).idft();
    {
      std::size_t i = 0;
      for(std::size_t j = 0; j < QQ.size(); j += 2) {
        QQ[i++] = QQ[j];
      }
      while(i < QQ.size()) QQ[i++] = T(0);
    }
    {
      std::size_t i = 0;
      for(std::size_t j = N % 2; j < PQ.size(); j += 2) {
        PQ[i++] = PQ[j];
      }
      while(i < PQ.size()) PQ[i++] = T(0);
    }
    P = std::move(PQ);
    Q = std::move(QQ);
    N /= 2;
  }
  return P[0] / Q[0];
}

#include "fps_ntt_multiply.hpp"
#include <iostream>

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  using fp = modint<998244353>;
  using mlt = fps_ntt_multiply<998244353, 3>;
  using fps = FPS<mlt>;

  i64 D, K;
  std::cin >> D >> K;

  vector<fp> a(D);
  for(int i = 0;i < D;i++) std::cin >> a[i].a;
  vector<fp> c(D + 1);
  for(int i = 1; i <= D; i++) std::cin >> c[i].a;
  c[0] = fp(-1);

  fps A(std::move(a));
  fps Q(std::move(c));
  i64 d = bound_pow2(D + 1);
  fps P = std::move(Q.clone().dft(d) *= std::move(A).dft(d)).idft(D);
  std::cout << mori_fps_division<mlt>(std::move(P), std::move(Q), K).a << std::endl;
}
