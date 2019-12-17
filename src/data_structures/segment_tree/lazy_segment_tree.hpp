#include <vector>
#include <set>
#include <iostream>
using i64 = long long;

struct lazy_segment_tree {
  using T = i64;
  using L = i64;
  static inline T t_ide() { return (1LL << 31) - 1; }
  static inline L l_ide() { return (1LL << 31) - 1; }
  static inline T ope(const T& a, const T& b) { return std::min(a, b); }
  static inline L lazy_ope(const L& a, const L& b) { return b; }
  static inline T effect(const T& t, const L& l) { return l; }

  int n, h;
  std::vector<T> node;
  std::vector<L> lazy;
  std::vector<bool> flag;

  lazy_segment_tree(int N) {
    n = 1;
    h = 1;
    while(n < N) n <<= 1, h++;
    node.resize(n << 1, t_ide());
    lazy.resize(n << 1, l_ide());
    flag.resize(n << 1, false);
  }
  lazy_segment_tree(const std::vector<T>& init) {
    n = 1;
    h = 1;
    while(n < init.size()) n <<= 1, h++;
    node.resize(n << 1, t_ide());
    lazy.resize(n << 1, l_ide());
    flag.resize(n << 1, false);
    for(int i = 0;i < init.size();i++) node[i + n] = init[i];
    for(int i = n; i --> 1;) node[i] = ope(node[(i << 1)], node[(i << 1) + 1]);
  }

  inline void eff(int k, L x) {
    if(k < n << 1) {
      lazy[k] = lazy_ope(lazy[k], x);
      flag[k] = true;
    }
  }
  inline T eval(int k) const { return flag[k] ? effect(node[k], lazy[k]) : node[k]; }

  inline void push(int k) {
    if(flag[k]) {
      node[k] = eval(k);
      eff(k << 1, lazy[k]);
      eff((k << 1) | 1, lazy[k]);
      lazy[k] = l_ide();
      flag[k] = false;
    }
  }

  inline void infuse(int k) {
    k = k >> __builtin_ctz(k);
    while((k >>= 1)) node[k] = ope(eval(k << 1), eval((k << 1) + 1));
  }

  inline void infiltrate(int k) {
    if(k == n << 1) return;
    int kc = __builtin_ctz(k);
    for(int i = h; i --> kc;) push(k >> i);
  }

  inline void infiltrate(int l, int r) {
    if(r == n << 1) infiltrate(l);
    else {
      int hh = h;
      int x = l ^ r;
      for(; !(x >> --hh);) push(l >> hh);
      int lc = __builtin_ctz(l);
      for(int i = hh + 1; i --> lc;) push(l >> i);
      int rc = __builtin_ctz(r);
      for(int i = hh + 1; i --> rc;) push(r >> i);
    }
  }

  void update(int a, int b, L x) {
    int l = a + n;
    int r = b + n;
    infiltrate(l, r);
    while(l < r) {
      if(l & 1) eff(l++, x);
      if(r & 1) eff(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }

  T sum(int l, int r) {
    l += n;
    r += n;
    infiltrate(l, r);
    T lx = t_ide();
    T rx = t_ide();
    while(l < r) {
      if(l & 1) lx = ope(lx, eval(l++));
      if(r & 1) rx = ope(eval(--r), rx);
      l >>= 1;
      r >>= 1;
    }
    return ope(lx, rx);
  }
};
