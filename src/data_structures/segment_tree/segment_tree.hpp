#include <vector>
using i64 = long long;

/**
 * # Segment Tree
 *
 * - モノイド列の点更新, 区間畳み込みが \\(\Theta(\log N)\\) でできるデータ構造
 * - 空間計算量 \\(\Theta(N)\\)
 * - ここでは, モノイドの演算が\\(\Theta(1)\\)であるとします.
 *
 * ## Template Arguments
 * ```cpp
 * @0@
 * ```
 *
 * - `T`: モノイドの型
 * - `Ope`: `operator()`でモノイドの演算を定義した構造体
 * - `Ide`: 零元
 *
 **/
template<class T, class Ope, const T& Ide>
struct segment_tree {
  Ope ope;
 
  i64 n;
  std::vector<T> node;
  
  /**
   * ## (constructor)
   * ```cpp
   * @0@
   * ```
   * 列`init`で初期化, 時間計算量 \\(\Theta(N)\\)
   **/
  segment_tree(const std::vector<T>& init) {
    n = 1;
    while(n < init.size()) n *= 2;
    node.resize(2 * n, Ide);
    for(int i = 0;i < init.size();i++) node[i + n] = init[i];
    for(int i = n - 1; i >= 1;i--) node[i] = ope(node[i * 2], node[i * 2 + 1]);
  }
 
  /**
   * ## update
   * ```cpp
   * @0@
   * ```
   * `A[i] <- x`とする. 時間計算量 \\(\Theta(\log N)\\)
   **/
  void update(i64 i, T x) {
    i += n;
    node[i] = x;
    while(i > 1) {
      i = i / 2;
      node[i] = ope(node[i * 2], node[i * 2 + 1]);
    }
  }

  /**
   * ## update
   * ```cpp
   * @0@
   * ```
   * \\([l, r)\\)の畳み込んだ値を返す. 時間計算量 \\(\Theta(\log N)\\)
   **/
  T sum(i64 l, i64 r) const {
    T lx = Ide;
    T rx = Ide;
    l += n;
    r += n;
    while(l < r) {
      if(l & 1) { lx = ope(lx, node[l++]); }
      if(r & 1) { rx = ope(node[--r], rx); }
      l >>= 1;
      r >>= 1;
    }
    return ope(lx, rx);
  }
};
