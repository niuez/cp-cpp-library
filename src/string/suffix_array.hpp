#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template<class Array>
struct suffix_array {
  Array arr;
  vector<i64> rank;
  vector<i64> tmp;
  vector<i64> sa;
  vector<i64> lcp;
  i64 n;

  /* O(nlog^2n) n = |S| */
  suffix_array(const Array& s): arr(s), rank(s.size() + 1), tmp(s.size() + 1), sa(s.size() + 1), n(s.size()), lcp(s.size() + 1) {
    for(int i = 0;i <= n;i++) {
      sa[i] = i;
      rank[i] = i < n ? s[i] : -1;
    }
    i64 k;
    auto compare_sa = [&](i64 i, i64 j) {
      if(rank[i] != rank[j]) return rank[i] < rank[j];
      else {
        i64 ri = (i + k <= n ? rank[i + k] : -1);
        i64 rj = (j + k <= n ? rank[j + k] : -1);
        return ri < rj;
      }
    };
    for(k = 1; k <= n; k *= 2) {
      sort(begin(sa), end(sa), compare_sa);
      tmp[sa[0]] = 0;
      for(int i = 1;i <= n;i++) {
        tmp[sa[i]] = tmp[sa[i - 1]] + !!(compare_sa(sa[i - 1], sa[i]));
      }
      for(int i = 0; i <= n; i++) {
        rank[i] = tmp[i];
      }
    }

    for(int i = 0; i <= n; i++) {
      rank[sa[i]] = i; 
    }

    i64 h = 0;
    lcp[0] = 0;
    for(int i = 0;i < n;i++) {
      int j = sa[rank[i] - 1];
      if(h > 0) h--;
      for(; j + h < n && i + h < n; h++) {
        if(arr[j + h] != arr[i + h]) break;
      }
      lcp[rank[i] - 1] = h;
    }
  }

  i64 SA(const i64 i) const {
    return sa[i];
  }

  i64 LCP(const i64 i) const {
    return lcp[i];
  }

  /* O(|T|logn) */
  /* return start index */
  /* not fount -> return -1 */
  i64 contain(const Array& t) {
    auto comp = [&](i64 s) {
      for(int i = s;i < s + t.size();i++) {
        if(i < arr.size()) {
          if(arr[i] != t[i - s]) {
            return t[i - s] - arr[i];
          }
        }
        else {
          return false;
        }
      }
      return 0;
    };
    i64 ok = n;
    i64 ng = 0;
    while(ok - ng > 1) {
      i64 m = (ok + ng) / 2;
      if(comp(sa[m]) > 0) { ng = m; }
      else { ok = m; }
    }
    if(comp(sa[ok]) == 0) {
      return sa[ok];
    }
    else {
      return -1;
    }
  }
};

/*
 * 23451, 34512, 45123, ...のなかから辞書順最小を見つける
 * s = 1234512345
 * s.suf[i] < s.suf[j] => rank[i] < rank[j]
 * sufの最初の5文字で最小は決められるため.
 * -> 条件に合うものをi = 0からsa[i]を探索
 */

