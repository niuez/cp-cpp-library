#include <vector>
#include <tuple>
#include <bitset>
#include <iostream>


struct union_find {
  std::vector<int> par;
  union_find(int N): par(N, -1) {}
  int root(int x) {
    return par[x] < 0 ? x : par[x] = root(par[x]);
  }
  std::tuple<int, int> unite(int x, int y) {
    x = root(x);
    y = root(y);
    if(x == y) return std::make_tuple(x, y);
    if(par[x] > par[y]) std::swap(x, y);
    par[x] += par[y];
    par[y] = x;
    return std::make_tuple(x, y);
  }
  int size(int x) {
    return -par[root(x)];
  }
};


struct incremental_bridge_connectivity {
  static const int MAX_N = 202020;
  static const int MAX_E = 202020;
  std::vector<int> par;
  std::vector<std::pair<int, int>> edge;
  std::bitset<MAX_N> bit;
  std::bitset<MAX_E> bridge;
  union_find cg;
  union_find bcg;

  incremental_bridge_connectivity(int N)
    : par(N, -1), cg(N), bcg(N) {
    }

  int lca(int u, int v) {
    bit = 0;
    while(1) {
      if(u >= 0) {
        if(bit.test(u)) {
          return u;
        }
        bit.set(u);
        if(par[u] < 0) u = -1;
        else u = edge[par[u]].second;
      }
      std::swap(u, v);
    }
  }

  void compress_path(int v, int w) {
    while(v != w) {
      bcg.unite(v, w);
      bridge.reset(par[v]);
      v = edge[par[v]].second;
    }
  }

  void reverse_path(int v, int p) {
    while(true) {
      std::swap(edge[p].first, edge[p].second);
      std::swap(p, par[v]);
      if(p < 0) break;
      v = edge[p].second;
    }
  }

  int add_edge(int u, int v) {
    int ei = edge.size();
    edge.emplace_back(u, v);
    if(bcg.root(u) == bcg.root(v)) {
    }
    else if(cg.root(u) == cg.root(v)) {
      int w = lca(u, v);
      compress_path(u, w);
      compress_path(v, w);
    }
    else {
      bridge.set(ei);
      if(cg.size(u) < cg.size(v)) {
        std::swap(u, v);
        std::swap(edge.back().first, edge.back().second);
      }
      reverse_path(v, ei);
      cg.unite(u, v);
    }
    return ei;
  }

  int size(int v) {
    return bcg.size(v);
  }

  bool is_bridge(int ei) {
    return bridge.test(ei);
  }
};

