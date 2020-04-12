#include <vector>
#include <queue>
using i64 = long long;

/*
 * delta(V v, fn (V t))
 * index(V v) -> int
 */
template<class V, class Delta, class Index>
std::vector<i64> bfs(std::size_t N, V s, Delta delta, Index index) {
  std::vector<i64> dist(N, -1);
  std::queue<V> que;
  dist[index(s)] = 0;
  que.push(s);
  while(!que.empty()) {
    V v = que.front();
    que.pop();
    delta(v, [&](V t) {
        if(dist[index(t)] == -1) {
          dist[index(t)] = dist[index(v)] + 1;
          que.push(t);
        }
    });
  }
  return dist;
}
