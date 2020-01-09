#include <vector>
using i64 = long long;

struct segment_tree_chmin_rsq {
  using T = i64;

  const T ide = 0;
  const T m_ide = -1e18;


  std::vector<T> sum;
  std::vector<T> m1, m2;
  std::vector<i64> mcnt;
  i64 n;
  i64 h;

  void fix(int k) {
    sum[k] = sum[k * 2 + 0] + sum[k * 2 + 1];
    if(m1[k * 2 + 1] < m1[k * 2 + 0]) {
      m1[k] = m1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0];
      m2[k] = std::max(m2[k * 2 + 0], m1[k * 2 + 1]);
    }
    else if(m1[k * 2 + 0] < m1[k * 2 + 1]) {
      m1[k] = m1[k * 2 + 1];
      mcnt[k] = mcnt[k * 2 + 1];
      m2[k] = std::max(m2[k * 2 + 1], m1[k * 2 + 0]);
    }
    else {
      m1[k] = m1[k * 2 + 0];
      mcnt[k] = mcnt[k * 2 + 0] + mcnt[k * 2 + 1];
      m2[k] = std::max(m2[k * 2 + 0], m2[k * 2 + 1]);
    }
  }

  segment_tree_chmin_rsq() {}
  segment_tree_chmin_rsq(const std::vector<i64>& vec) {
    n = 1;
    h = 1;
    while(n < vec.size()) n <<= 1, h++;
    sum.resize(2 * n);
    m1.resize(2 * n);
    m2.resize(2 * n, m_ide);
    mcnt.resize(2 * n, 0);
    for(i64 i = 0;i < vec.size();i++) {
      sum[i + n] = vec[i];
      m1[i + n] = vec[i];
      mcnt[i + n] = 1;
    }
    for(i64 i = n; i --> 1;) {
      fix(i);
    }
  }


  void eff(int k, T x) {
    sum[k] += (x - m1[k]) * mcnt[k];
    m1[k] = x;
  }

  void push(int k) {
    if(m1[k] < m1[k * 2 + 0]) eff(k * 2 + 0, m1[k]);
    if(m1[k] < m1[k * 2 + 1]) eff(k * 2 + 1, m1[k]);
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
    if(m1[k] <= x) return;
    if(m2[k] < x) {
      eff(k, x);
      return;
    }
    push(k);
    subtree_chmin(k * 2 + 0, x);
    subtree_chmin(k * 2 + 1, x);
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
  seg.range_chmin(0, 16, 5);
  cout << seg.range_sum(5, 10) << endl;
}
