#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct eulartour_path {
  vector<vector<i64>> G;
  vector<i64> in, out;
  i64 cnt;
  eulartour_path(i64 n): G(n), in(n), out(n) {}
  void add_edge(i64 u, i64 v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }

  void dfs(i64 v, i64 f) {
    for(auto to: G[v]) {
      if(to == f) continue;
      in[to] = cnt;
      cnt++;
      dfs(to, v);
      out[to] = cnt;
      cnt++;
    }
  }

  void start_tour(i64 r) {
    in[r] = cnt;
    cnt++;
    dfs(r, -1);
  }

  i64 edge_in(i64 v) { return in[v]; }
  i64 edge_out(i64 v) { return out[v]; }
  pair<i64, i64> path_range(i64 u, i64 v) {
    return { in[u] + 1, in[v] + 1 };
  }
};


