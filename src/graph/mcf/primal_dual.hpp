#include <vector>
#include <queue>
#include <set>
using namespace std;

struct edge_base { int from; int to;
}; template<class E>
struct graph_base {
  virtual void add_edge(E e) = 0;
  virtual const vector<E>& operator[](int i) const = 0;
  virtual int N() const = 0;
};

template<class E>
struct graph : graph_base<E> {
  vector<vector<E>> G;
  graph(int n) : G(n) {}
  void add_edge(E e) {
    G[e.from].push_back(e);
  }
  const vector<E>& operator[](int i) const { return G[i]; }
  vector<E>& operator[](int i) { return G[i]; }
  int N() const { return G.size(); }
};

template<class C>
struct net_cost_edge : edge_base {
  C cap;
  C cost;
  int rev;
  net_cost_edge(int f,int t,C c,C w,int r) {
    from = f;
    to = t;
    cap = c;
    cost = w;
    rev = r;
  }
};

template<class C>
struct network_cost : public graph<net_cost_edge<C>> {
  const int s;
  const int t;
  template<class E, class F, class CF>
  network_cost(const graph<E>& g,int s,int t, F f,CF cost) : graph<net_cost_edge<C>>(g.N()), s(s) ,t(t) {
    for(int i = 0;i < g.N();i++) {
      for(auto& e : g[i]) {
        this->add_edge(net_cost_edge<C>(e.from,e.to,f(e),cost(e),this->G[e.to].size()));
        this->add_edge(net_cost_edge<C>(e.to,e.from,C(),-cost(e),this->G[e.from].size() - 1));
      }
    }
  }
};

template<class C>
pair<bool,C> primal_dual_mcf(network_cost<C>& g, C f) {
  int s = g.s;
  int t = g.t;
  int n = g.N();
  vector<C> po(n,C());
  C ans = C();
  auto sel = [&](net_cost_edge<C> e) {
    return e.cost + po[e.from] - po[e.to];
  };
  while(f > 0) {
    vector<C> dist(n,C());
    vector<bool> vis(n,false);
    vector<int> p(n,-1);
    vector<int> pe(n,-1);
    using P = pair<C,int>;
    priority_queue<P,vector<P>,greater<P>> que;
    que.push({dist[s] ,s});
    vis[s] = true;
    while(!que.empty()) {
      int v = que.top().second;
      C d = que.top().first;
      que.pop();
      if(dist[v] < d) continue;
      for(int i = 0;i < g[v].size();i++) {
        auto& e = g[v][i];
        int u = e.to;
        if(e.cap == C()) continue;
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

    C ff = f;
    for(int u = t;u != s;u = p[u])
      ff = min(ff, g[p[u]][pe[u]].cap);
    for(int u = t;u != s;u = p[u]) {
      ans += ff * g[p[u]][pe[u]].cost;
      g[p[u]][pe[u]].cap -= ff;
      g[u][g[p[u]][pe[u]].rev].cap += ff;
    }
    f -= ff;
    for(int i = 0;i < n;i++)
      po[i] += dist[i];
  }
  if(f != C()) return {false,C()};
  return {true ,ans };
}
