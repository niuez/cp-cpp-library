#include <vector>
#include <queue>
#include <set>
using i64 = long long;

struct dinic {
  using capacity_type = i64;
  struct edge {
    i64 from;
    i64 to;
    capacity_type cap;
    capacity_type cost;
    i64 rev;
  };
  i64 n;
  i64 s, t;
  std::vector<std::vector<edge>> g;

  dinic(i64 n, i64 s, i64 t): n(n), s(s), t(t), g(n) {}
  void add_edge(i64 from, i64 to, i64 cap, i64 cost) {
    g[from].push_back({ from, to, cap, cost, (i64)(g[to].size()) });
    g[to].push_back({ to, from, 0, -cost, (i64)(g[from].size() - 1) });
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

  capacity_type min_cost_flow(capacity_type f) {
    std::vector<capacity_type> po(n,0);
    capacity_type ans = 0;
    auto sel = [&](const edge& e) {
      return e.cost + po[e.from] - po[e.to];
    };
    while(f > 0) {
      std::vector<capacity_type> dist(n,0);
      std::vector<bool> vis(n,false);
      std::vector<int> p(n,-1);
      std::vector<int> pe(n,-1);
      using P = std::pair<capacity_type,int>;
      std::priority_queue<P,std::vector<P>,std::greater<P>> que;
      que.push({dist[s] ,s});
      vis[s] = true;
      while(!que.empty()) {
        int v = que.top().second;
        capacity_type d = que.top().first;
        que.pop();
        if(dist[v] < d) continue;
        for(int i = 0;i < g[v].size();i++) {
          auto& e = g[v][i];
          int u = e.to;
          if(e.cap == 0) continue;
          if(!vis[u] || dist[u] > dist[v] + sel(e)) {
            vis[u] = true;
            dist[u] = dist[v] + sel(e);
            p[u] = v;
            pe[u] = i;
            que.push({dist[u] , u});
          }
        }
      }
      if(p[t] == -1) break;

      capacity_type ff = f;
      for(int u = t;u != s;u = p[u])
        ff = std::min(ff, g[p[u]][pe[u]].cap);
      for(int u = t;u != s;u = p[u]) {
        ans += ff * g[p[u]][pe[u]].cost;
        g[p[u]][pe[u]].cap -= ff;
        g[u][g[p[u]][pe[u]].rev].cap += ff;
      }
      f -= ff;
      for(int i = 0;i < n;i++)
        po[i] += dist[i];
    }
    if(f != 0) return -1;
    return ans;
  }
};

