#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

vector<i64> strongly_connected_components(const vector<vector<i64>>& g) {
  i64 n = g.size();
  vector<vector<i64>> rg(n);
  for(int i = 0;i < n;i++) {
    for(auto j: g[i]) {
      rg[j].push_back(i);
    }
  }

  vector<bool> vis(n,false);
  vector<i64> vs;
  vector<i64> res(n,-1);
  function<void(int)> dfs = [&](int v) {
    vis[v] = true;
    for(auto& t : g[v]) {
      if(!vis[t]) dfs(t);
    }
    vs.push_back(v);
  };

  function<void(int,int)> rdfs = [&](int v,int k) {
    vis[v] = true;
    res[v] = k;
    for(auto to : rg[v]) {
      if(!vis[to]) rdfs(to,k);
    }
  };

  for(int i = 0;i < n;i++) {
    if(!vis[i]) dfs(i);
  }

  vis.assign(n,false);
  int k = 0;
  for(int i = n - 1;i >= 0;i--) {
    if(!vis[vs[i]]) rdfs(vs[i] , k++);
  }
  return res;
}
