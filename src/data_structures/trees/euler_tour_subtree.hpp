#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct eulertour_subtree {
  vector<vector<i64>> G;
  vector<i64> tour;
  vector<i64> L, R;
  vector<i64> depth;
  eulertour_subtree(i64 n): G(n), L(n), R(n), depth(n) {}
  void add_edge(i64 u, i64 v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }

  void dfs(i64 v, i64 f, i64 d) {
    tour.push_back(v);
    L[v] = tour.size() - 1;
    depth[v] = d;
    for(auto to: G[v]) {
      if(to == f) continue;
      dfs(to, v, d + 1);
      //tour.push_back(v);
    }
    R[v] = tour.size() - 1;
  }

  void start_tour(i64 r) {
    dfs(r, -1, 0);
  }

  //[L[v], R[v])
  pair<i64, i64> subtree_range(i64 v) {
    return pair<i64, i64>(L[v], R[v]);
  }

  i64 vertex(i64 v) {
    return L[v];
  }
};
