# FPS Division

- \\( P(x) / Q(x) \\) の \\( [x^N] \\)の係数を求める
- 時間計算量は \\( O( k \log k \log N) \\)
- \\( P(x) \\)は高々\\( k - 1 \\)次の多項式
- \\( Q(x) \\)は\\( k \\)次の多項式
# Code

```cpp
#include "formal_power_series.hpp"


template<class T, class fps_multiply>
T mori_fps_division(FPS<T, fps_multiply> P, FPS<T, fps_multiply> Q, std::size_t N) {
  P.bound_resize();
  Q.bound_resize();
  i64 k = std::max(P.size(), Q.size());
  P.coef.resize(k * 2);
  Q.coef.resize(k * 2);
  while(N > 0) {
    auto Qm = Q;
    for(std::size_t i = 1; i < Qm.size(); i += 2) {
      Qm[i] *= T(-1);
    }
    auto Pd = fps_multiply::dft(std::move(P.coef));
    auto Qd = fps_multiply::dft(std::move(Q.coef));
    auto Qmd = fps_multiply::dft(std::move(Qm.coef));
    auto PQ = fps_multiply::idft(
        fps_multiply::multiply(std::move(Pd), Qmd)
        );
    auto QQ = fps_multiply::idft(
        fps_multiply::multiply(std::move(Qd), std::move(Qmd))
        );
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
    P.coef = std::move(PQ);
    Q.coef = std::move(QQ);
    N /= 2;
  }
  return P[0] / Q[0];
}
```
