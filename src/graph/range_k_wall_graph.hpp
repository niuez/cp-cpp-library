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
