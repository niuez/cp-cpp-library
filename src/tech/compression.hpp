#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template<class T>
struct Compression {
  vector<T> v;

  Compression(){}
  void add(const T& t) { v.push_back(t); }
  void build() {
    sort(begin(v), end(v));
    v.erase(unique(begin(v), end(v)), end(v));
  }

  i64 comp(const T& x) {
    return lower_bound(begin(v), end(v), x) - begin(v);
  }

  vector<i64> comp(vector<T> a) {
    vector<i64> ans;
    for(auto& x: a)
      ans.push_back(comp(x));
    return ans;
  }

  i64 size() const { return v.size(); }
};
