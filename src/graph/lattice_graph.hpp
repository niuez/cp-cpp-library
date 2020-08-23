#include <set>
#include <vector>
using i64 = long long;

template<class F>
struct lattice_delta {
  i64 H, W;
  F f;
  using P = std::pair<i64, i64>;
  lattice_delta(i64 H, i64 W, F f): H(H), W(W), f(f) {}
  template<class Func>
  void operator()(P v, Func func) {
    const static std::vector<i64> dx { 1, 0, -1, 0 };
    i64 i = v.first;
    i64 j = v.second;
    for(i64 q = 0; q < 4; q++) {
      i64 x = i + dx[q];
      i64 y = j + dx[q ^ 1];
      if(0 <= x && x < H && 0 <= y && y < W) {
        f(P(i, j), P(x, y), func);
      }
    }
  }
};
 
template<class F>
lattice_delta<F> make_lattice_delta(i64 H, i64 W, F f) { return lattice_delta<F>(H, W, f); }
 
struct lattice_index {
  i64 H, W;
  using P = std::pair<i64, i64>;
  lattice_index(i64 H, i64 W): H(H), W(W) {}
  i64 operator()(P v) {
    return v.first * W + v.second;
  }
};
