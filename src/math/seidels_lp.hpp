#include <vector>
#include <set>
#include <cassert>
#include <limits>
#include <cmath>
#include <iostream>

/**
 * # Seidel's LP algorithm
 * - LPを解くアルゴリズム
 * - LP(線形計画問題)とは, 以下のように記述できる.
 * 
 * \\[
 * \begin{align}
 * \text{maximize} \ & c^T x \\\\
 * \text{subject to} \ & A x \leq b \\\\
 *                   \ & x \geq 0
 * \end{align}
 * \\]
 *
 * - この \\( x \\) を返す.
 * - 計算量 \\( O(d! m) ( d \text{は次元数}, m \text{は条件数}) \\)
 * 
 * ## Arguments 
 *
 * ```cpp
 * @3@
 * ```
 * `mat`は, \\( (A \ b) \\)である.
 *
 * ```cpp
 * @4@
 * ```
 * `bounds`には, \\( x \\)のとる下界と上界を与える.
 *
 * ## Memo
 *
 * ```cpp
 * @5@
 * ```
 * `eps`を変える必要があるかもしれない.
 **/
template<class R, class T = long double>
std::vector<T> seidel_lp(R& rnd, std::size_t d,
    const std::vector<T>& c,
    const std::vector<std::vector<T>>& mat,
    const std::vector<std::pair<T, T>>& bounds) {
  const static T eps = std::numeric_limits<T>::epsilon();
  const static auto eps_eq = [&](const T& a, const T& b) -> bool {
    return std::abs(a - b) <= eps;
  };
  assert(d > 0);
  if(d == 1) {
    assert(c.size() == 1);
    T low = bounds[0].first;
    T high = bounds[0].second;
    T z = T(0);
    for(const auto& a: mat) {
      assert(a.size() == 2);
      if(eps_eq(a[0], T(0))) {
        // equal
        if(std::abs(a[1] - z) <= eps || a[1] < z) z = a[1];
      }
      else if(a[0] > T(0)) {
        // greater
        T pa = a[1] / a[0];
        if(eps_eq(pa, high) || pa < high) high = pa;
      }
      else {
        T pa = a[1] / a[0];
        if(eps_eq(pa, low) || pa > low) low = pa;
      }
    }
    if(z < T(0) || high < low) return std::vector<T>();
    else if(eps_eq(c[0], T(0)) || c[0] > T(0)) return std::vector<T> { high };
    else return std::vector<T> { low };
  }
  else if(mat.empty()) {
    std::vector<T> res(d);
    for(int i = 0; i < d; i++) {
      if(eps_eq(c[i], T(0)) || c[i] > T(0)) res[i] = bounds[i].second;
      else res[i] = bounds[i].first;
    }
    return res;
  }
  else {
    int rmi = rnd() % mat.size();
    const auto& a = mat[rmi];
    std::vector<std::vector<T>> next_mat(mat.size() - 1);
    {
      int ni = 0;
      for(int i = 0; i < mat.size(); i++) {
        if(i == rmi) continue;
        next_mat[ni++] = mat[i];
      }
    }
    auto v = seidel_lp(rnd, d, c, next_mat, bounds);
    if(v.empty()) return v;
    {
      T value = T(0);
      for(int i = 0; i < d; i++) {
        value += a[i] * v[i];
      }
      if(eps_eq(value, a[d]) || value < a[d]) return v;
    }
    int k = -1;
    for(int i = 0; i < d; i++) {
      if(!eps_eq(a[i], T(0))) k = i;
    }
    if(k == -1) return std::vector<T>();
    
    std::vector<std::pair<T, T>> next_bounds(d - 1);
    {
      int ni = 0;
      for(int i = 0;i < d; i++) {
        if(i == k) continue;
        next_bounds[ni++] = bounds[i];
      }
    }
    std::vector<std::vector<T>> bar_mat(next_mat.size() + 2, std::vector<T>(d));
    for(int mi = 0; mi < next_mat.size(); mi++) {
      auto ratio = next_mat[mi][k] / a[k];
      int ni = 0;
      for(int i = 0; i <= d; i++) {
        if(i == k) continue;
        bar_mat[mi][ni++] = next_mat[mi][i] - ratio * a[i];
      }
    }
    std::vector<T> bar_c(d - 1);
    {
      auto ratio = c[k] / a[k];
      int ni = 0;
      for(int i = 0; i < d; i++) {
        if(i == k) continue;
        bar_c[ni++] = c[i] - ratio * a[i];
      }
    }
    {
      int ni = 0;
      for(int i = 0; i < d; i++) {
        if(i == k) continue;
        bar_mat[next_mat.size()][ni] = - (T(1) / a[k]) * a[i];
        bar_mat[next_mat.size() + 1][ni++] = - (T(1) / a[k]) * a[i];
      }
      bar_mat[next_mat.size()][d - 1] = bounds[k].second;
      bar_mat[next_mat.size() + 1][d - 1] = bounds[k].first;
    }

    v = seidel_lp(rnd, d - 1, bar_c, bar_mat, next_bounds);
    if(v.empty()) {
      return v;
    }
    else {
      v.insert(std::begin(v) + k, T(0));
      T s = 0;
      for(int i = 0; i < d; i++) s += a[i] * v[i];
      v[k] = (a[d] - s) / a[k];
      return v;
    }
  }
}

#include <random>
#include <iostream>
#include <iomanip>

int main() {
  long double C, D;
  std::cin >> C >> D;
  auto r = std::mt19937(9982);
  std::vector<long double> c { 1.0, 2.0 };
  std::vector<std::vector<long double>> mat {
    { 3.0 / 4.0, 2.0 / 7.0, C },
    { 1.0 / 4.0, 5.0 / 7.0, D },
    { -1.0, 0.0, 0.0 },
    { 0.0, -1.0, 0.0 }
  };
  std::vector<std::pair<long double, long double>> bounds {
    { 0.0, 2000.0 },
    { 0.0, 2000.0 }
  };
  auto ans = seidel_lp(r, 2, c, mat, bounds);
  std::cout << std::fixed << std::setprecision(10) << ans[0] * 1000.0 + ans[1] * 2000.0 << std::endl;
}
