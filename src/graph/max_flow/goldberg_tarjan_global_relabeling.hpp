#include <vector>
#include <iostream>

struct goldberg_tarjan {
  using cap_type = int;
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
  const cap_type INF = 1e9;

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
    h[v] = N - 1;
    for(const auto& e: G[v]) {
      if(e.cap > 0 && h[v] > h[e.to]) {
        h[v] = h[e.to];
      }
    }
    h[v]++;
  }

  std::vector<int> Q;
  void global_relabeling(int t) {
    int qi = 0, qr = 0;
    Q[qr++] = t;
    h.assign(N, N - 1);
    h[t] = 0;
    while(qi < qr) {
      int v = Q[qi++];
      for(const auto& e: G[v]) {
        if(G[e.to][e.rev].cap > 0 && h[v] + 1 < h[e.to]) {
          h[e.to] = h[v] + 1;
          Q[qr++] = e.to;
        }
      }
    }
  }

  cap_type max_flow(int s, int t) {
    exc.assign(N, 0);
    exc[s] = INF;
    h.assign(N, 0);
    Q.resize(N);
    int cnt = 0;

    global_relabeling(t);

    std::vector<std::vector<int>> que(N);
    std::vector<int> qi(N);
    que[h[s]].push_back(s);

    for(int di = h[s]; di >= 0;) {
      if(qi[di] == que[di].size()) { di--; continue; }
      int v = que[di][qi[di]++];
      if(exc[v] == 0 || v == t) continue;
      for(auto& e: G[v]) {
        if(e.cap > 0 && h[v] == h[e.to] + 1) {
          push(v, e);
          if(exc[e.to] > 0 && e.to != t) {
            que[h[e.to]].push_back(e.to);
          }
        }
      }
      if(exc[v] == 0) continue;
      relabel(v);
      if(h[v] < N) {
        di = h[v];
        que[h[v]].push_back(v);
      }

      if(++cnt % N == 0) {
        global_relabeling(t);
      }
    }
    return exc[t];
  }
};


#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define all(x) x.begin(),x.end()

template<class T>
static inline std::vector<T> ndvec(size_t&& n, T val) noexcept {
  return std::vector<T>(n, std::forward<T>(val));
}

template<class... Tail>
static inline auto ndvec(size_t&& n, Tail&&... tail) noexcept {
  return std::vector<decltype(ndvec(std::forward<Tail>(tail)...))>(n, ndvec(std::forward<Tail>(tail)...));
}

template<class T, class Cond>
struct chain {
  Cond cond; chain(Cond cond) : cond(cond) {}
  bool operator()(T& a, const T& b) const {
    if(cond(a, b)) { a = b; return true; }
    return false;
  }
};
template<class T, class Cond>
chain<T, Cond> make_chain(Cond cond) { return chain<T, Cond>(cond); }


int main() {
  cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  int L, R, M;
  cin >> L >> R >> M;
  goldberg_tarjan flw(L + R + 2);
  rep(i,0,L) {
    flw.add_edge(L + R, i, 1);
  }
  rep(i,0,R) {
    flw.add_edge(i + L, L + R + 1, 1);
  }
  rep(i,0,M) {
    int a, b;
    cin >> a >> b;
    flw.add_edge(a, b + L, 1);
  }
  cout << flw.max_flow(L + R, L + R + 1) << '\n';
  for(int i = 0;i < L;i++) {
    for(const auto& e: flw.G[i]) {
      if(e.cap == 0 && e.to < L + R) {
        cout << i << " " << e.to - L << '\n';
      }
    }
  }
}
