#include <vector>
 
template<class T, class E, class U, class To, class Adj, class Ope, class Merge>
std::vector<T> rerooting(const std::vector<std::vector<E>>& G, int r, U ide, To to, Adj adj, Ope ope, Merge merge) {
  int N = G.size();
  std::vector<T> tdp(N);
  std::vector<int> par(N, -1);
  std::vector<int> que(N);
  int qi = 0;
  que[qi++] = r;
 
  int MAX = 1;
  for(int i = 0;i < qi;i++) {
    int v = que[i];
    for(const auto& e: G[v]) {
      int t = to(e);
      if(t == par[v]) continue;
      par[t] = v;
      que[qi++] = t;
    }
    MAX = std::max(MAX, (int)G[v].size() + 1);
  }
 
  for(int i = qi; i --> 0;) {
    int v = que[i];
    U sum = ide;
    for(const auto& e : G[v]) {
      int t = to(e);
      if(t == par[v]) continue;
      sum = ope(sum, adj(tdp[t], e));
    }
    tdp[v] = merge(sum, v);
  }
 
  std::vector<U> f(MAX);
  std::vector<U> b(MAX);
 
  for(int i = 0; i < qi; i++) {
    int v = que[i];
    for(int j = 0;j < G[v].size();j++) {
      int t = to(G[v][j]);
      f[j + 1] = adj(tdp[t == par[v] ? v : t], G[v][j]);
    }
    b[G[v].size()] = ide;
    for(int j = G[v].size(); j --> 0;) b[j] = ope(f[j + 1], b[j + 1]);
    f[0] = ide;
    for(int j = 0;j < G[v].size(); j++) {
      int t = to(G[v][j]);
      f[j + 1] = ope(f[j], f[j + 1]);
      if(t != par[v]) {
        tdp[t] = merge(ope(f[j], b[j + 1]), v);
      }
    }
    tdp[v] = merge(f[G[v].size()], v);
  }
  return tdp;
}
