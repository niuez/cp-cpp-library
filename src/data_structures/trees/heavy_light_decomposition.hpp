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
