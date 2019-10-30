#include <vector>
#include <cassert>
#include <set>
#include <queue>
#include <functional>
#include <algorithm>

struct edge {
  int to;
  int label;
};


::std::vector<::std::pair<int, int>> gabow_edmonds(const ::std::vector<::std::vector<int>>& gra){
  // E0 - 初期化
  int N = gra.size();
  ::std::vector<::std::vector<edge>> g(N + 1);
  ::std::vector<::std::pair<int, int>> edges;
  {
    int cnt = N + 1;
    for(int i = 0;i < N;i++){
      for(auto to : gra[i]){
        if(i < to){
          g[to + 1].push_back({i + 1, cnt});
          g[i + 1].push_back({to + 1, cnt++});
          edges.push_back({i + 1, to + 1});
        }
      }
    }
  }
  ::std::vector<int> mate(N + 1 , 0);
  ::std::vector<int> label(N + 1 , -1);
  ::std::vector<int> first(N + 1 , 0);
  ::std::queue<int> que;

  // firstの遅延評価
  ::std::function<int(int)> eval_first = [&](int x){
    if(label[first[x]] < 0) return first[x];
    first[x] = eval_first(first[x]);
    return first[x];
  };

  // サブルーチンR
  ::std::function<void(int, int)> rematch = [&](int v, int w){
    // R1
    int t = mate[v];
    mate[v] = w;
    if(mate[t] != v) return;
    // R2
    if(label[v] <= N){
      mate[t] = label[v];
      rematch(label[v] , t);
    }
    // R3
    else{
      int x = edges[label[v] - N - 1].first;
      int y = edges[label[v] - N - 1].second;
      rematch(x , y);
      rematch(y , x);
    }
  };

  ::std::function<void(int, int, int)> assignLabel = [&](int x, int y, int num){
    // L0
    int r = eval_first(x);
    int s = eval_first(y);
    int join = 0;
    if(r == s) return;
    // -numがフラグ
    label[r] = -num;
    label[s] = -num;
    while(true){
      // L1
      if(s != 0) ::std::swap(r , s);
      // L2
      r = eval_first(label[mate[r]]);
      if(label[r] == -num){
        join = r;
        break;
      }
      label[r] = -num;
    }
    // L3
    int v = first[x];
    // L4
    while(v != join){
      que.push(v);
      label[v] = num;
      first[v] = join;
      v = first[label[mate[v]]];
    }
    // L3
    v = first[y];
    // L4
    while(v != join){
      que.push(v);
      label[v] = num;
      first[v] = join;
      v = first[label[mate[v]]];
    }
    // L5は遅延評価しているため不要
    // L6
    return;
  };

  ::std::function<bool(int)> augment_check = [&](int u){
    // E1 後半
    first[u] = 0;
    label[u] = 0;
    que.push(u);
    while(!que.empty()){
      // E2 
      int x = que.front();
      que.pop();
      for(auto e : g[x]){
        int y = e.to;
        // E3
        if(mate[y] == 0 && y != u){
          mate[y] = x;
          rematch(x , y);
          return true;
        }
        // E4
        else if(label[y] >= 0){
          assignLabel(x , y , e.label);
        }
        // E5
        else if(label[mate[y]] < 0){
          label[mate[y]] = x;
          first[mate[y]] = y;
          que.push(mate[y]);
        }
        // E6
      }
    }
    return false;
  };

  for(int i = 1;i <= N;i++){
    // E1
    que = ::std::queue<int>();
    if(mate[i] != 0) continue;
    if(augment_check(i)){
      // E7
      ::std::fill(label.begin(), label.end(), -1);
    }
  }

  ::std::vector<::std::pair<int, int>> ans;
  for(int i = 1;i <= N;i++){
    if(i < mate[i]){
      ans.push_back({i , mate[i]});
    }
  }
  return ans;
}
