#include <vector>
#include <queue>

/*
 * delta(V v, fn (V t, W weight))
 * index(V v) -> int
 */
template<class V, class W, class Delta, class Index>
std::vector<W> dijkstra(std::size_t N, W inf, V s, Delta delta, Index index) {
  std::vector<W> dist(N, inf);
  using P = std::pair<W, V>;
  std::priority_queue<P, std::vector<P>, std::greater<P>> que;
  que.push({ dist[index(s)] = W(), s });
  while(!que.empty()) {
    W d = que.top().first;
    V v = que.top().second;
    que.pop();
    if(dist[index(v)] < d) continue;
    delta(v, [&](V t, W weight) {
        if(dist[index(t)] > dist[index(v)] + weight) {
          que.push({ dist[index(t)] = dist[index(v)] + weight, t });
        }
    });
  }
  return dist;
}
