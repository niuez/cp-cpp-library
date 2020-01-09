#include <vector>
#include <limits>
using i64 = long long;

struct segment_tree_chminmax_rsq {
  using T = i64;

  const T ide = 0;
  const T INF = std::numeric_limits<T>::max();
  const T NINF = std::numeric_limits<T>::min();


  std::vector<T> sum;
  std::vector<T> gst1, gst2;
  std::vector<T> lst1, lst2;
  std::vector<i64> mcnt;
  i64 n;
  i64 h;

  void fix(int k) {
    sum[k] = sum[k * 2 + 0] + sum[k * 2 + 1];

    if(gst1[k * 2 + 1] < gst1[k * 2 + 0]) {
      gst1[k] = gst1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0];
      gst2[k] = std::max(gst2[k * 2 + 0], gst1[k * 2 + 1]);
    }
    else if(gst1[k * 2 + 0] < gst1[k * 2 + 1]) {
      gst1[k] = gst1[k * 2 + 1];
      mcnt[k] = mcnt[k * 2 + 1];
      gst2[k] = std::max(gst2[k * 2 + 1], gst1[k * 2 + 0]);
    }
    else {
      gst1[k] = gst1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0] + mcnt[k * 2 + 1];
      gst2[k] = std::max(gst2[k * 2 + 0], gst2[k * 2 + 1]);
    }

    if(lst1[k * 2 + 0] < lst1[k * 2 + 1]) {
      lst1[k] = lst1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0];
      lst2[k] = std::min(lst2[k * 2 + 0], lst1[k * 2 + 1]);
    }
    else if(lst1[k * 2 + 1] < lst1[k * 2 + 0]) {
      lst1[k] = lst1[k * 2 + 1];
      mcnt[k] = mcnt[k * 2 + 1];
      lst2[k] = std::min(lst2[k * 2 + 1], lst1[k * 2 + 0]);
    }
    else {
      lst1[k] = lst1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0] + mcnt[k * 2 + 1];
      lst2[k] = std::min(lst2[k * 2 + 0], lst2[k * 2 + 1]);
    }
  }

  segment_tree_chmin_rsq() {}
  segment_tree_chmin_rsq(const std::vector<i64>& vec) {
    n = 1;
    h = 1;
    while(n < vec.size()) n <<= 1, h++;
    sum.resize(2 * n);
    gst1.resize(2 * n);
    gst2.resize(2 * n, NINF);
    lst1.resize(2 * n);
    lst2.resize(2 * n, INF);
    mcnt.resize(2 * n, 0);
    for(i64 i = 0;i < vec.size();i++) {
      sum[i + n] = vec[i];
      gst1[i + n] = vec[i];
      lst1[i + n] = vec[i];
      mcnt[i + n] = 1;
    }
    for(i64 i = n; i --> 1;) {
      fix(i);
    }
  }

  void eff_chmin(int k, T x) {
    sum[k] += (x - gst1[k]) * mcnt[k];
    if(gst1[k] == lst1[k]) {
      gst1[k] = lst1[k] = x;
    }
    else if(gst1[k] == lst2[k]) {
      gst1[k] = lst2[k] = x;
    }
    else {
      gst1[k] = x;
    }
  }

  void eff_chmax(int k, T x) {
    sum[k] += (x - lst1[k]) * mcnt[k];
    if(lst1[k] == gst1[k]) {
      lst1[k] = gst1[k] = x;
    }
    else if(lst1[k] == gst2[k]) {
      lst1[k] = gst2[k] = x;
    }
    else {
      lst1[k] = x;
    }
  }

  void push(int k) {
    if(gst1[k] < gst1[k * 2 + 0]) eff_chmin(k * 2 + 0, gst1[k]);
    if(gst1[k] < gst1[k * 2 + 1]) eff_chmin(k * 2 + 1, gst1[k]);

    if(lst1[k] > lst1[k * 2 + 0]) eff_chmax(k * 2 + 0, gst1[k]);
    if(lst1[k] > lst1[k * 2 + 1]) eff_chmax(k * 2 + 1, gst1[k]);
  }

  void infuse(int k) {
    k = k >> __builtin_ctz(k);
    while(k >>= 1) fix(k);
  }

  void infiltrate(int k) {
    if(k == n << 1) return;
    for(int i = h; i --> 1;) push(k >> i);
  }

  void subtree_chmin(int k, T x) {
    if(gst1[k] <= x) return;
    if(gst2[k] < x) {
      eff_chmin(k, x);
      return;
    }
    push(k);
    subtree_chmin(k * 2 + 0, x);
    subtree_chmin(k * 2 + 1, x);
    fix(k);
  }

  void subtree_chmax(int k, T x) {
    if(x <= lst1[k]) return;
    if(x < lst2[k]) {
      eff_chmax(k, x);
      return;
    }
    push(k);
    subtree_chmax(k * 2 + 0, x);
    subtree_chmax(k * 2 + 1, x);
    fix(k);
  }

  void range_chmin(int a, int b, T x) {
    infiltrate(a + n);
    infiltrate(b + n);
    int l = a + n;
    int r = b + n;
    while(l < r) {
      if(l & 1) subtree_chmin(l++, x);
      if(r & 1) subtree_chmin(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }

  void range_chmax(int a, int b, T x) {
    infiltrate(a + n);
    infiltrate(b + n);
    int l = a + n;
    int r = b + n;
    while(l < r) {
      if(l & 1) subtree_chmax(l++, x);
      if(r & 1) subtree_chmax(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }

  T range_sum(int l, int r) {
    l += n;
    r += n;
    infiltrate(l);
    infiltrate(r);
    T lx = ide;
    T rx = ide;
    while(l < r) {
      if(l & 1) lx = lx + sum[l++];
      if(r & 1) rx = sum[--r] + rx;
      l >>= 1;
      r >>= 1;
    }
    return lx + rx;
  }
};

#include <iostream>
using std::cout;
using std::endl;

int main() {
  segment_tree_chmin_rsq seg(std::vector<i64>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });

  cout << seg.range_sum(1, 5) << endl;
  cout << seg.range_sum(6, 10) << endl;
  seg.range_chmax(0, 5, 2);
  seg.range_chmin(0, 5, 3);
  cout << seg.range_sum(0, 5) << endl;
}
