#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>

struct capacity_scaling_ssp_mcf {
  using capacity_type = long long;
  using weight_type = long long;
  using size_type = std::size_t;


  struct edge {
    size_type to;
    size_type rev;
    capacity_type cap;
    capacity_type flow;
    weight_type weight;
    capacity_type residual_cap() const {
      return cap - flow;
    }
  };

  struct edge_ptr {
    size_type from;
    size_type idx;
  };

  size_type n;
  std::vector<std::vector<edge>> g;
  std::vector<capacity_type> b;
  std::vector<weight_type> phi;

  capacity_scaling_ssp_mcf(size_type n)
    : n(n),
      g(n),
      b(n),
      phi(n) {
  }

  edge_ptr add_edge(size_type from, size_type to, capacity_type lower, capacity_type upper, weight_type weight) {
    size_type fidx = g[from].size();
    size_type tidx = g[to].size() + !!(from == to);
    g[from].push_back(edge { to, tidx, upper, capacity_type(0), weight });
    g[to].push_back(edge { from, fidx, -lower, capacity_type(0), -weight });
    return edge_ptr { from, fidx };
  }

  void add_supply(size_type v, const capacity_type& amount) {
    b[v] += amount;
  }

  void add_demand(size_type v, const capacity_type& amount) {
    b[v] -= amount;
  }

  const capacity_type& get_edge_flow(const edge_ptr& e) {
    return g[e.from][e.idx].flow;
  }

  const weight_type& get_potential(size_type v) {
    return phi[v];
  }

private:

  capacity_type get_delta(const capacity_type& scaling_factor) const {
    capacity_type cap_inf = *std::max_element(std::begin(b), std::end(b));
    for(const auto& es: g) for(const auto& e: es) {
      cap_inf = std::max({ cap_inf, e.cap, -e.cap });
    }
    capacity_type delta = capacity_type(1);
    while(delta < cap_inf) { delta *= scaling_factor; }
    return delta;
  }

  void push(edge& e, const capacity_type& amount) {
    e.flow += amount;
    g[e.to][e.rev].flow -= amount;
  }

  weight_type residual_weight(size_type from, const edge& e) const {
    return e.weight + phi[from] - phi[e.to];
  }

  void saturate_negative(const capacity_type& delta) {
    for(size_type v = 0; v < n; v++) {
      for(auto& e: g[v]) {
        capacity_type cap = e.residual_cap();
        cap -= cap % delta;
        if(cap < 0 || residual_weight(v, e) < 0) {
          push(e, cap);
          b[v] -= cap;
          b[e.to] += cap;
        }
      }
    }
  }

  void dual(size_type excess_cnt, const std::vector<size_type>& excess_vs, const capacity_type& delta, std::vector<edge_ptr>& par, std::vector<size_type>& dificit_vs) {
    using P = std::pair<weight_type, size_type>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> que;
    std::vector<weight_type> dist(n, std::numeric_limits<weight_type>::max());
    weight_type farthest = 0;
    for(size_type i = 0; i < excess_cnt; i++) {
      size_type v = excess_vs[i];
      dist[v] = 0;
      que.emplace(dist[v], v);
    }
    while(!que.empty()) {
      weight_type d = que.top().first;
      size_type v = que.top().second;
      que.pop();
      if(dist[v] < d) continue;
      farthest = d;
      if(b[v] <= -delta) dificit_vs.push_back(v);
      for(size_type ei = 0; ei < g[v].size(); ei++) {
        const auto& e = g[v][ei];
        if(e.residual_cap() < delta) continue;
        weight_type wgh = d + residual_weight(v, e);
        if(dist[e.to] > wgh) {
          dist[e.to] = wgh;
          par[e.to] = edge_ptr { v, ei };
          que.push({ wgh, e.to });
        }
      }
    }
    for(size_type i = 0; i < n; i++) {
      phi[i] += std::min(dist[i], farthest);
    }
  }

  void primal(const std::vector<edge_ptr>& par, const std::vector<size_type>& dificit_vs, const capacity_type& delta) {
    for(const auto& t: dificit_vs) {
      capacity_type f = -b[t];
      size_type v = t;
      while(par[v].idx != std::numeric_limits<size_type>::max()) {
        f = std::min(f, g[par[v].from][par[v].idx].residual_cap());
        v = par[v].from;
      }
      f = std::min(f, b[v]);
      f -= f % delta;
      if(f <= 0) continue;
      v = t;
      while(par[v].idx != std::numeric_limits<size_type>::max()) {
        push(g[par[v].from][par[v].idx], f);
        v = par[v].from;
      }
      b[t] += f;
      b[v] -= f;
    }
  }
  bool remove_not_excess(std::vector<size_type>& excess_vs, size_type& excess_cnt, const capacity_type& delta) {
    for(size_type i = 0; i < excess_cnt;) {
      if(b[excess_vs[i]] < delta) {
        std::swap(excess_vs[i], excess_vs[--excess_cnt]);
      }
      else {
        i++;
      }
    }
    return excess_cnt > 0;
  }
public:
  bool solve(const capacity_type& scaling_factor) {
    std::vector<size_type> excess_vs(n);
    size_type excess_cnt;
    std::vector<size_type> dificit_vs;
    dificit_vs.reserve(n);
    std::vector<edge_ptr> par(n);
    for(capacity_type delta = get_delta(scaling_factor);
        delta > 0;
        delta /= scaling_factor) {
      std::iota(std::begin(excess_vs), std::end(excess_vs), 0);
      excess_cnt = n;
      saturate_negative(delta);
      while(remove_not_excess(excess_vs, excess_cnt, delta)) {
        dificit_vs.clear();
        std::fill(std::begin(par), std::end(par), edge_ptr { 0, std::numeric_limits<size_type>::max() });
        dual(excess_cnt, excess_vs, delta, par, dificit_vs);
        if(dificit_vs.size() > 0) {
          primal(par, dificit_vs, delta);
        }
        else {
          break;
        }
      }
    }
    return std::count(std::begin(b), std::end(b), 0) == n;
  }

  template<class Cost>
  Cost get_result() const {
    Cost result = 0;
    for(const auto& es: g) for(const auto& e: es) {
      result += Cost(e.flow) * Cost(e.weight);
    }
    return result / 2;
  }
};
