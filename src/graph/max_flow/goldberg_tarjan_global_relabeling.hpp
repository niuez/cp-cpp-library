#include <vector>
#include <iostream>

struct goldberg_tarjan {
  using cap_type = long long;
  struct edge {
    int to;
    cap_type cap;
    int rev;
    edge(int t, cap_type c, int r): to(t), cap(c), rev(r) {}
  };

  int N;
  std::vector<std::vector<edge>> G;
  std::vector<cap_type> exc;
  std::vector<int> h;
  std::vector<std::vector<int>> que;
  std::vector<int> qi;
  std::vector<int> ei;
  const cap_type INF = 1e18;

  goldberg_tarjan(int n): N(n), G(n) {}
  
  void add_edge(int from, int to, cap_type cap, cap_type rev_cap = 0) {
    G[from].emplace_back(to, cap, (int)(G[to].size()));
    G[to].emplace_back(from, rev_cap, (int)(G[from].size() - 1));
  }

  void push(int from, edge& e) {
    cap_type fl = std::min(exc[from], e.cap);
    e.cap -= fl;
    G[e.to][e.rev].cap += fl;
    exc[from] -= fl;
    exc[e.to] += fl;
  }

  void relabel(int v) {
    h[v] = N;
    for(int i = 0; i < G[v].size(); i++) {
      auto& e = G[v][i];
      if(e.cap > 0 && h[v] > h[e.to] + 1) {
        ei[v] = i;
        h[v] = h[e.to] + 1;
      }
    }
  }

  std::vector<int> Q;
  int global_relabeling(int t) {
    for(int i = 0;i < N;i++) {
      que[i].clear();
      qi[i] = 0;
      ei[i] = 0;
    }
    int i = 0, qr = 0;
    Q[qr++] = t;
    h.assign(N, N);
    h[t] = 0;
    int hi = 0;
    while(i < qr) {
      int v = Q[i++];
      hi = h[v];
      if(exc[v] > 0 && v != t) {
        que[h[v]].push_back(v);
      }
      for(const auto& e: G[v]) {
        if(G[e.to][e.rev].cap > 0 && h[v] + 1 < h[e.to]) {
          h[e.to] = h[v] + 1;
          Q[qr++] = e.to;
        }
      }
    }
    return hi;
  }

  cap_type max_flow(int s, int t) {
    exc.assign(N, 0);
    exc[s] = INF;
    h.assign(N, 0);
    Q.resize(N);
    int cnt = 0;

    que.assign(N, std::vector<int>());
    qi.assign(N, 0);
    ei.assign(N, 0);

    global_relabeling(t);

    if(h[s] == N) return 0;

    for(int di = h[s]; di >= 0;) {
      if(qi[di] == que[di].size()) { di--; continue; }
      int v = que[di][qi[di]++];
      if(exc[v] == 0 || v == t) continue;
      for(int& i = ei[v]; i < G[v].size(); i++) {
        auto& e = G[v][i];
        if(e.cap > 0 && h[v] == h[e.to] + 1) {
          push(v, e);
          if(exc[e.to] > 0 && e.to != t) {
            que[h[e.to]].push_back(e.to);
          }
          if(exc[v] == 0) break;
        }
      }
      if(exc[v] == 0) continue;
      relabel(v);
      if(h[v] < N) {
        di = h[v];
        que[h[v]].push_back(v);
      }

      if(++cnt % N == 0) {
        di = global_relabeling(t);
      }
    }
    return exc[t];
  }
};
