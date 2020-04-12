#include <vector>
#include <queue>
using i64 = long long;

/*
 * delta(V v, fn (V t, i64 weight))
 * index(V v) -> int
 */
template<class V, class W, class Delta, class Index>
std::vector<i64> dial01(std::size_t N, W inf, V s, Delta delta, Index index) {
  std::vector<i64> dist(N, inf);
  using P = std::pair<V, W>;
  std::deque<P> que;
  dist[index(s)] = 0;
  que.push_back({ s, dist[index(s)]});
  while(!que.empty()) {
    V v = que.front().first;
    W d = que.front().second;
    que.pop_front();
    if(dist[index(v)] < d) continue;
    delta(v, [&](V t, i64 weight) {
        if(dist[index(t)] > dist[index(v)] + weight) {
          dist[index(t)] = dist[index(v)] + weight;
          if(weight == 0) que.push_front({ t, dist[index(t)] });
          else que.push_back({ t, dist[index(t)] });
        }
    });
  }
  return dist;
}
