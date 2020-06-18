# Range-K Wall Graph

[ABC170F - Pond Skater](https://atcoder.jp/contests/abc170/tasks/abc170_f)で使ったテク

- 各頂点から区間\\( K \\)の中にある頂点に辺を貼る.
- 貼るために必要な空間量は\\( O(N) \\)
- `[start, end)`の頂点で作る



## (constructor)

```cpp
range_k_wall_graph_builder(int N, int offset, int K)
```

- `N`: 貼りたい頂点の数
- `offset`: 新たに作る頂点番号の始めの番号(`start')を指定する
- `K`: 区間
## init

```cpp
template<class Wall, class Idx, class Connect>
```

ハシゴを作る.

- `wall(v) -> bool`: 頂点番号を受け取り, 壁であればtrueを返す
- `idx(i) -> V`: 貼りたい頂点の数`N`に対応した頂点の番号を返す
- `con(v, w)`: `(v, w)`の辺を貼るというコールバック
## build_for_right

各頂点\\(i (0 \leq i \le N)\\)から, \\([i, i + K]\\)に辺を貼る.

## build_for_right

各頂点\\(i (0 \leq i \le N)\\)から, \\([i - K, i]\\)に辺を貼る.

# Code

```cpp
#include <vector>



struct range_k_wall_graph_builder {
  int N;
  int start;
  int end;
  int K;
  

  range_k_wall_graph_builder(int N, int offset, int K)

    : N(N),
      start(offset),
      end(offset + N * 2),
      K(K) {
  }

  range_k_wall_graph_builder(){}


  template<class Wall, class Idx, class Connect>
  void init(Wall wall, Idx idx, Connect con) {
    for(int i = 0; i < N; i++) {
      int v = idx(i);
      if(wall(v)) continue;
      con(start + i, v);
      con(start + N + i, v);
      if(i + 1 < N && (i + 1) % K != 0) {
        con(start + i, start + i + 1);
        con(start + N + i + 1, start + N + i);
      }
    }
  }
  

  template<class Wall, class Idx, class Connect>
  void build_for_right(Wall wall, Idx idx, Connect con) {
    int wi = N;
    for(int i = N; i --> 0;) {
      int v = idx(i);
      if(wall(v)) wi = i;
      else {
        con(v, start + i);
        int ni = std::min(i + K, wi - 1);
        if(i / K != ni / K) {
          con(v, start + N + ni);
        }
      }
    }
  }


  template<class Wall, class Idx, class Connect>
  void build_for_left(Wall wall, Idx idx, Connect con) {
    int wi = -1;
    for(int i = 0; i < N; i++) {
      int v = idx(i);
      if(wall(v)) wi = i;
      else {
        con(v, start + N + i);
        int ni = std::max(i - K, wi + 1);
        if(i / K != ni / K) {
          con(v, start + ni);
        }
      }
    }
  }
};
```
