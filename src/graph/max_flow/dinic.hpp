#include <vector>
#include <queue>
using i64 = long long;

struct dinic {
  using capacity_type = i64;
  struct edge {
    i64 from;
    i64 to;
    capacity_type cap;
    i64 rev;
  };
  i64 n;
  i64 s, t;
  std::vector<std::vector<edge>> g;

  dinic(i64 n, i64 s, i64 t): n(n), s(s), t(t), g(n) {}
  void add_edge(i64 from, i64 to, i64 cap) {
    g[from].push_back({ from, to, cap, (i64)(g[to].size()) });
    g[to].push_back({ to, from, 0, (i64)(g[from].size() - 1) });
  }
  std::vector<int> level;
  std::vector<int> iter;
  capacity_type dinic_dfs(int v, capacity_type f) {
    if(v == t) return f;
    else {
      capacity_type now = f;
      for(int& i = iter[v]; i < g[v].size(); i++) {
        auto& e = g[v][i];
        if(e.cap > 0 && level[e.to] > level[e.from]) {
          capacity_type c = std::min(now , e.cap);
          capacity_type d = dinic_dfs(e.to, c);
          e.cap -= d;
          g[e.to][e.rev].cap += d;
          now -= d;
          if(now == 0) return f - now;
        }
      }
      return f - now;
    }
  }

  capacity_type max_flow() {
    capacity_type ans = 0;
    capacity_type inf = 0;
    for(auto& e : g[s]) {
      inf += e.cap;
    }
    while(1) {
      std::queue<int> que;
      std::vector<int> level(n,-1);
      que.push(s);
      level[s] = 0;
      while(!que.empty()) {
        int v = que.front();
        que.pop();
        for(auto& e : g[v]) {
          if(e.cap > 0 && level[e.to] == -1) {
            level[e.to] = level[e.from] + 1;
            que.push(e.to);
          }
        }
      }
      if(level[t] < 0) break;
      capacity_type f;
      while((f = dinic_dfs(s, inf)) > 0) {
        ans += f;
      }
    }
    return ans;
  }
};

