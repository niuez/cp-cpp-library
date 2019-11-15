#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define rev(i,s,e) for(i64 (i) = (e); (i)-- > (s);)
#define all(x) x.begin(),x.end()

struct Mo {
  vector<i64> left, right, order;
  vector<bool> v;
  

  const i64 width;
  i64 nl, nr, ptr;

  vector<i64> a;
  vector<i64> cnt;
  i64 ans;

  Mo(i64 n, vector<i64> a) : v(n), width((i64)sqrt(n)), nl(0), nr(0), ptr(0), a(a), cnt(1010101), ans(0) {}

  void add_query(i64 l, i64 r) {
    left.push_back(l);
    right.push_back(r);
  }

  void build() {
    order.resize(left.size());
    for(i64 i = 0;i < left.size();i++) order[i] = i;
    sort(begin(order), end(order), [&](i64 a, i64 b) {
        if(left[a] / width != left[b] / width) return left[a] < left[b];
        else return right[a] < right[b];
        });
  }


  void add(i64 idx) {
    if(cnt[a[idx]]++ == 0) ans++;
  }
  void del(i64 idx) {
    if(--cnt[a[idx]] == 0) ans--;
  }

  inline void distribute(i64 idx) {
    v[idx].flip();
    if(v[idx]) add(idx);
    else del(idx);
  }

  i64 process() {
    if(ptr == order.size()) return -1;
    const auto id = order[ptr];
    while(nl > left[id]) distribute(--nl);
    while(nr < right[id]) distribute(nr++);
    while(nl < left[id]) distribute(nl++);
    while(nr > right[id]) distribute(--nr);
    return order[ptr++];
  }
};
