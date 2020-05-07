#include <vector>
#include <queue>

struct dinic {
  using cap_type = int;
  const cap_type INF = 1e9;
  struct edge {
    int to;
    cap_type cap;
    int rev;
  };
  int n;
  std::vector<std::vector<edge>> g;

  dinic(int n): n(n), g(n) {}

  void add_edge(int from, int to, cap_type cap, cap_type rev_cap = 0) {
    g[from].push_back({ to, cap, (int)(g[to].size()) });
    g[to].push_back({ from, rev_cap, (int)(g[from].size() - 1) });
  }
  
  std::vector<int> level;
  std::vector<int> iter;
  cap_type dfs(const int s, const int v, cap_type mf) {
    if(s == v || mf == 0) return mf;
    for(int& i = iter[v]; i < g[v].size(); i++) {
      int t = g[v][i].to;
      edge& re = g[v][i];
      edge& e = g[t][re.rev];
      if(level[t] >= level[v] || e.cap == 0) continue;
      cap_type f = dfs(s, t, std::min(mf, e.cap));
      if(f == 0) continue;
      e.cap -= f;
      re.cap += f;
      return f;
    }
    return 0;
  }

  cap_type max_flow(int s, int t) {
    std::vector<int> que(n);
    cap_type flow = 0;
    while(true) {
      level.assign(n, -1);
      int qi = 0;
      int qr = 0;
      level[s] = 0;
      que[qr++] = s;
      while(qi < qr && level[t]) {
        int v = que[qi++];
        for(const auto& e: g[v]) {
          if(e.cap > 0 && level[e.to] == -1) {
            level[e.to] = level[v] + 1;
            que[qr++] = e.to;
          }
        }
      }
      if(level[t] == -1) break;
      iter.assign(n, 0);
      cap_type tmp;
      while((tmp = dfs(s, t, INF)) > 0) {
        flow += tmp;
      }
    }
    return flow;
  }
};
