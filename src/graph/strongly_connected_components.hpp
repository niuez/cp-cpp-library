#include <vector>
#include <iostream>
 
struct strongly_connected_components {
  std::vector<std::vector<int>> g;
  std::vector<std::vector<int>> rg;
  std::vector<int> vis;
  std::vector<int> vs;
 
  std::vector<int> group;
  std::vector<std::vector<int>> comps;
  int groups;
 
  strongly_connected_components(int N): g(N), rg(N), vis(N, 0), group(N, -1) {}
 
  void add_edge(int a, int b) {
    g[a].push_back(b);
    rg[b].push_back(a);
  }
 
  void dfs(int v) {
    vis[v] = 1;
    for(auto& t: g[v]) {
      if(!vis[t]) dfs(t);
    }
    vs.push_back(v);
  }
 
  void rdfs(int v, int k) {
    vis[v] = 1;
    group[v] = k;
    comps.back().push_back(k);
    for(auto to: rg[v]) {
      if(!vis[to]) rdfs(to, k);
    }
  }
 
  void build() {
    for(int i = 0; i < g.size(); i++) {
      if(!vis[i]) dfs(i);
    }
    vis.assign(g.size(), false);
    groups = 0;
    for(int i = g.size(); i --> 0; ) {
      if(!vis[vs[i]]) {
        comps.push_back(std::vector<int>());
        rdfs(vs[i], groups++);
      }
    }
  }
 
  std::vector<std::vector<int>> build_compressed_graph() {
    std::vector<std::vector<int>> cg(groups);
    for(int i = 0; i < g.size(); i++) {
      for(auto& j: g[i]) {
        cg[group[i]].push_back(group[j]);
      }
    }
    return cg;
  }
};
