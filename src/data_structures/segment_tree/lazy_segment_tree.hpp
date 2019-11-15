#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct lazy_segment_tree {
  using T = i64;
  using L = pair<i64, i64>;
  static T t_ide() { return 0; }
  static L l_ide() { return {0, 0}; }
  static T ope(const T& a, const T& b) { return a + b; }
  static L lazy_ope(const L& a, const L& b) { return { a.first + b.first, a.second + b.second }; }
  static T effect(const T& t, const L& l, const i64 len) { return t + l.first * len; }

  i64 n;
  vector<T> node;
  vector<L> lazy;
  vector<bool> flag;

  /* create lazy_segment_tree for sequence [t_ide; sz]A */
  lazy_segment_tree(const vector<T>& init) {
    n = 1;
    while(n < init.size()) n *= 2;
    node.resize(2 * n - 1, t_ide());
    lazy.resize(2 * n - 1, l_ide());
    flag.resize(2 * n - 1, false);
    for(int i = 0;i < init.size();i++) node[i + n - 1] = init[i];
    for(int i = n - 2; i >= 0;i--) node[i] = ope(node[i * 2 + 1], node[i * 2 + 2]);
  }
  

  void eval(i64 l, i64 r, i64 k) {
    if(flag[k]) {
      node[k] = effect(node[k], lazy[k], r - l);
      
      if(r - l > 1) {
        lazy[k * 2 + 1] = lazy_ope(lazy[k * 2 + 1], lazy[k]);
        flag[k * 2 + 1] = true;
        lazy[k * 2 + 2] = lazy_ope(lazy[k * 2 + 2], lazy[k]);
        flag[k * 2 + 2] = true;
      }

      flag[k] = false;
      lazy[k] = l_ide();
    }
  }
  
  /* for i in [a, b) effect(a[i], lx) */
  void update(i64 a, i64 b, L lx, i64 l = 0, i64 r = -1, i64 k = 0) {
    if(r < 0) r = n;
    eval(l, r, k);
    if(b <= l || r <= a) return;
    else if(a <= l && r <= b) {
      lazy[k] = lazy_ope(lazy[k], lx);
      flag[k] = true;
      eval(l, r, k);
    }
    else {
      update(a, b, lx, l, (l + r) / 2, k * 2 + 1);
      update(a, b, lx, (l + r) / 2, r, k * 2 + 2);
      node[k] =ope(node[k * 2 + 1], node[k * 2 + 2]);
    }
  }
  
  /* the sum of a[i] for i in [a, b) */
  T sum(i64 a, i64 b, i64 l = 0, i64 r = -1, i64 k = 0) {
    if(r < 0) r = n;
    eval(l, r, k);
    if(b <= l || r <= a) return t_ide();
    else if(a <= l && r <= b) return node[k];
    else return ope(sum(a, b, l, (l + r) / 2, k * 2 + 1), sum(a, b, (l + r) / 2, r, k * 2 + 2));
  }
};
