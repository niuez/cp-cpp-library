#include <vector>

template<class G>
struct CentroidDecomposition {
  const G& g;
  std::vector<bool> deleted;
  std::vector<int> sz;
  std::vector<int> par;
  CentroidDecomposition(int N, const G& g): g(g), deleted(N, false), sz(N), par(N) {}
  int calc_size(int v, int p) {
    sz[v] = 1;
    for(int u: g[v]) {
      if(deleted[u] || u == p) continue;
      sz[v] += calc_size(u, v);
    }
    return sz[v];
  }

  void centroid_build(int v, int p) {
    int tot = calc_size(v, -1);
    bool ok = false;
    int pp = -1;
    while(!ok) {
      ok = true;
      for(int u: g[v]) {
        if(!deleted[u] && u != pp && 2 * sz[u] > tot) {
          pp = v;
          v = u;
          ok = false;
          break;
        }
      }
    }
    par[v] = p;
    // operation
    deleted[v] = true;
    for(int u: g[v]) {
      if(!deleted[u]) {
        centroid_build(u, v);
      }
    }
  }
};
