#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct HeavyLightDecomposition {
  vector<vector<i64>> G;
  vector<i64> in, out;
  vector<i64> sz;
  vector<i64> seq;
  vector<i64> next;
  vector<i64> par;


  HeavyLightDecomposition(i64 n)
    : G(n), in(n), out(n), sz(n), next(n, -1), par(n, -1) {}

  void add_edge(i64 u, i64 v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }

  void dfs_sz(i64 v, i64 f) {
    sz[v] = 1;
    for(i64 i = 0;i < G[v].size();i++) {
      i64 x = G[v][i];
      if(x == f) continue;
      dfs_sz(x, v);
      par[x] = v;
      sz[v] += sz[x];
      if(sz[G[v][0]] < sz[G[v][i]]) { swap(G[v][0], G[v][i]); }
    }
  }

  i64 dfs_eul(i64 v, i64 f, i64 t) {
    in[v] = t++;
    seq.push_back(v);
    for(i64 i = 0;i < G[v].size();i++) {
      i64 x = G[v][i];
      if(x == f) continue;
      next[x] = (i == 0) ? next[v] : x;
      t = dfs_eul(x, v, t);
    }
    return out[v] = t;
  }

  void build(i64 r) {
    dfs_sz(r, -1);
    dfs_eul(r, -1, 0);
  }

  const vector<i64>& sequence() const { return seq; }
  i64 lca(i64 a, i64 b) const {
    while(true) {
      if(in[b] > in[a]) swap(a, b);
      if(next[b] == next[a]) return b;
      a = par[next[a]];
    }
  }

  pair<vector<pair<i64, i64>>, vector<pair<i64, i64>>> path(i64 a, i64 b, bool edge) const {
    vector<pair<i64, i64>> l, r;
    while(true) {
      if(in[b] > in[a]) { swap(a, b); swap(l, r); }
      if(next[b] == next[a]) {
        l.push_back({ in[b] + !!edge, in[a] + 1 });
        break;
      }
      l.push_back({ in[next[a]], in[a] + 1 });
      a = par[next[a]];
    }
    return { std::move(l), std::move(r) };
  }

  pair<i64, i64> subtree(i64 v, bool edge) { return { in[v] + !!edge, out[v] }; }
};

struct lazy_segment_tree {
  using T = i64;
  using L = i64;
  static T t_ide() { return 0; }
  static L l_ide() { return 0; }
  static T ope(const T& a, const T& b) { return a + b; }
  static L lazy_ope(const L& a, const L& b) { return a + b; }
  static T effect(const T& t, const L& l, const i64 len) { return t + l * len; }

  i64 n;
  vector<T> node;
  vector<L> lazy;
  vector<bool> flag;

  /* create lazy_segment_tree for sequence [t_ide; sz]A */
  lazy_segment_tree(const vector<T>& init) {
    n = 1;
    while(n < init.size()) n *= 2;
    node.resize(2 * n - 1);
    lazy.resize(2 * n - 1);
    flag.resize(2 * n - 1);
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


int main() {
  i64 n;
  cin >> n;
  HeavyLightDecomposition hld(n);
  vector<i64> w(n);
  for(int i = 0;i < n - 1;i++) {
    i64 a, b, c;
    cin >> a >> b >> c;
    w[b] = c;
    hld.add_edge(a, b);
  }
  hld.build(0);
  vector<i64> init(n, 0);
  for(i64 i = 0;i < n;i++) { init[i] = w[hld.seq[i]]; }

  lazy_segment_tree seg(init);

  i64 Q;
  cin >> Q;
  for(int q = 0;q < Q;q++) {
    i64 type;
    cin >> type;
    if(type == 1) {
      i64 a, x;
      cin >> a >> x;
      auto p = hld.subtree(a, true);
      seg.update(p.first, p.second, x);
    }
    else {
      i64 b;
      cin >> b;
      i64 sum = 0;
      auto p = hld.path(0, b, true);
      for(auto x: p.first) {
        sum += seg.sum(x.first, x.second);
      }
      for(auto x: p.second) {
        sum += seg.sum(x.first, x.second);
      }
      cout << sum << endl;
    }
  }
}
