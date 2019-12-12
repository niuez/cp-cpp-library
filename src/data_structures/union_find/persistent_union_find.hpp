#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct partially_persistent_union_find {
  vector<i64> data;
  vector<i64> last;
  vector<vector<pair<i64, i64>>> add;

  partially_persistent_union_find(int sz) : data(sz, -1), last(sz, 1e9), add(sz) {
    for(auto & vs: add) vs.push_back({ -1, -1 });
  }

  i64 unite(i64 t, i64 x, i64 y) {
    x = find(t, x);
    y = find(t, y);
    if(x == y) return -1;
    if(data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    add[x].push_back({t, data[x]});
    data[y] = x;
    last[y] = t;
    return x;
  }

  i64 find(i64 t, i64 x) {
    if(t < last[x]) return x;
    return find(t, data[x]);
  }

  i64 size(i64 t, i64 x) {
    x = find(t, x);
    return -prev(lower_bound(begin(add[x]), end(add[x]), make_pair(t, 0ll)))->second;
  }
};
