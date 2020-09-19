#include <vector>

struct strongly_connected_componects {
  int n;
  std::vector<std::vector<int>> G;
  std::vector<std::vector<int>> scc;
  std::vector<int> low;
  std::vector<int> num;
  std::vector<int> st;
  std::vector<bool> in_st;
  int cnt;

  strongly_connected_componects(int n):
    n(n), G(n) {}

  void add_edge(int a, int b) {
    G[a].emplace_back(b);
  }

  void visit(int v) {
    low[v] = num[v] = ++cnt;
    st.emplace_back(v);
    in_st[v] = true;

    for(auto t: G[v]) {
      if(num[t] == 0) {
        visit(t);
        low[v] = std::min(low[v], low[t]);
      }
      else if(in_st[t]) {
        low[v] = std::min(low[v], num[t]);
      }
    }

    if(low[v] == num[v]) {
      std::vector<int> vs;
      while(true) {
        int t = st.back();
        st.pop_back();
        in_st[t] = false;
        vs.emplace_back(t);
        if(v == t) break;
      }
      scc.emplace_back(std::move(vs));
    }
  }

  void build_scc() {
    low.assign(n, 0);
    num.assign(n, 0);
    st.reserve(n);
    in_st.assign(n, false);
    cnt = 0;
    for(int i = 0;i < n;i++) {
      if(num[i] == 0) {
        visit(i);
      }
    }
  }
};

