#include <vector>
#include <queue>
using namespace std;

struct edge_base {
  int from;
  int to;
};

template<class E>
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
struct net_edge : edge_base {
  C cap;
  int rev;
  net_edge(int f,int t,C c,int r) {
    from = f;
    to = t;
    cap = c;
    rev = r;
  }
};

template<class C>
struct network : public graph<net_edge<C>> {
  const int s;
  const int t;
  template<class E, class CF>
  network(const graph<E>& g,int s,int t, CF f) : graph<net_edge<C>>(g.N()), s(s) ,t(t) {
    for(int i = 0;i < g.N();i++) {
      for(auto& e : g[i]) {
        this->add_edge(net_edge<C>(e.from,e.to,f(e),this->G[e.to].size()));
        this->add_edge(net_edge<C>(e.to,e.from,C(),this->G[e.from].size() - 1));
      }
    }
  }
};

template<class C>
C dinic_dfs(int v,network<C>& net, const vector<int>& level, C f) {
  if(v == net.t) return f;
  else {
    C now = f;
    for(auto& e : net[v]) {
      if(e.cap > C() && level[e.to] > level[e.from]) {
        C c = min(now , e.cap);
        C d = dinic_dfs(e.to,net,level,c);
        e.cap -= d;
        net[e.to][e.rev] += d;
        now -= d;
      }
    }
    return f - now;
  }
}

template<class C>
C dinic_Mflow(network<C>& net){
  C ans = C();
  C inf = C();
  int s = net.s;
  int t = net.t;
  int n = net.N();
  for(auto& e : net[s]) {
    inf += e.cap;
  }
  while(1) {
    queue<int> que;
    vector<int> level(n,-1);
    que.push(s);
    level[s] = 0;
    while(!que.empty()) {
      int v = que.front();
      que.pop();
      for(auto& e : net[v]) {
        if(e.cap > C() && level[e.to] == -1) {
          level[e.to] = level[e.from] + 1;
          que.push(e.to);
        }
      }
    }
    if(level[t] < 0) break;
    C f;
    while((f = dinic_dfs(s,net,level,inf)) > C()) {
      ans += f;
    }
  }
  return ans;
}
