#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template<class T>
struct dynamic_slide_min {
  const i64 k;
  deque<pair<i64, T>> que;
  i64 i;
  /* k := length of slide */
  dynamic_slide_min(const i64 k): k(k), i(0) {}
  /* return min value in slide k after inserting x */
  T next(T x) {
    while(!que.empty() && que.back().second >= x) que.pop_back();
    que.push_back({i, x});
    i64 min_i = que.front().second;
    i++;
    if(i == que.front().first + k) {
      que.pop_front();
    }
    return min_i;
  }
};
