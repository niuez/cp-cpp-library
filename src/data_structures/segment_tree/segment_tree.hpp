#include <vector>
using i64 = long long;

template<class T, class Ope, const T& Ide>
struct segment_tree {
  Ope ope;
 
  i64 n;
  std::vector<T> node;
 
  segment_tree(const std::vector<T>& init) {
    n = 1;
    while(n < init.size()) n *= 2;
    node.resize(2 * n, Ide);
    for(int i = 0;i < init.size();i++) node[i + n] = init[i];
    for(int i = n - 1; i >= 1;i--) node[i] = ope(node[i * 2], node[i * 2 + 1]);
  }
 
  void modify(i64 i, T x) {
    i += n;
    node[i] = ope(node[i], x);
    while(i > 1) {
      i = i / 2;
      node[i] = ope(node[i * 2], node[i * 2 + 1]);
    }
  }

  /* [l, r) */
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
