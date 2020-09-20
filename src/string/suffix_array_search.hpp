#include <vector>
#include <iostream>

template<class T>
const std::vector<int>& sa_is(std::vector<T> s, int k) {
  int N = s.size();
  static std::vector<int> sa;
  static std::vector<int> cnt;
  sa.resize(N + 1);

  if(N == 0) return sa;

  for(auto& c: s) c++;
  s.push_back(0);
  k++;

  std::vector<bool> iss(N + 1);
  std::vector<int> lms;
  std::vector<int> is_lms(N + 1, -1);
  std::vector<int> bin(k + 1);

  iss[N] = true;
  bin[1]++;
  for(int i = N; i --> 0; ) {
    if(s[i] == s[i + 1])
      iss[i] = iss[i + 1];
    else
      iss[i] = s[i] < s[i + 1];
    if(!iss[i] && iss[i + 1]) {
      is_lms[i + 1] = lms.size();
      lms.push_back(i + 1);
    }
    bin[s[i] + 1]++;
  }

  for(int i = 1;i <= k;i++)
    bin[i] += bin[i - 1];

  auto induce = [&](const std::vector<int>& lms) {
    sa.assign(N + 1, -1);
    cnt.assign(k, 0);

    for(int i = 0;i < lms.size();i++) {
      int x = lms[i];
      sa[bin[s[x] + 1] - 1 - cnt[s[x]]] = x;
      cnt[s[x]]++;
    }

    cnt.assign(k, 0);
    for(int i = 0;i <= N;i++) {
      int x = sa[i] - 1;
      if(x >= 0 && !iss[x]) {
        sa[bin[s[x]] + cnt[s[x]]] = x;
        cnt[s[x]]++;
      }
    }

    cnt.assign(k, 0);
    for(int i = N + 1;i --> 0;) {
      int x = sa[i] - 1;
      if(x >= 0 && iss[x]) {
        sa[bin[s[x] + 1] - 1 - cnt[s[x]]] = x;
        cnt[s[x]]++;
      }
    }
  };

  induce(lms);


  if(lms.size() >= 2) {
    int M = lms.size();
    int li = 0;
    std::vector<int> rec_lms(M);
    for(auto x: sa) {
      if(is_lms[x] != -1) rec_lms[li++] = x;
    }
    int rec_n = 1;
    std::vector<int> rec_s(M);
    rec_s[M - 1 - is_lms[rec_lms[1]]] = 1;
    for(int i = 2;i < M;i++) {
      int xl = rec_lms[i];
      int yl = rec_lms[i - 1];
      int xr = lms[is_lms[xl] - 1];
      int yr = lms[is_lms[yl] - 1];
      if(xr - xl != yr - yl)
        rec_n++;
      else {
        while(xl <= xr) {
          if(s[xl] != s[yl]) {
            rec_n++;
            break;
          }
          xl++;
          yl++;
        }
      }
      rec_s[M - 1 - is_lms[rec_lms[i]]] = rec_n;
    }


    sa_is(std::move(rec_s), rec_n + 1);
    li = M;
    for(int i = 1;i < M + 1;i++) {
      rec_lms[--li] = lms[M - 1 - sa[i]];
    }
    induce(rec_lms);
  }

  return sa;
}

template<class T>
struct suffix_array_search {
  int N;
  std::vector<T> str;
  std::vector<int> rnk;
  std::vector<int> sa;
  std::vector<int> lcp;


  std::vector<int> seg;
  int seg_n;

  //sparse_table<int> st;
  
  template<class I>
  suffix_array_search(I b, I e, int k): str(b, e) {
    N = str.size();
    sa = sa_is(str, k);
    rnk.resize(N + 1);
    lcp.resize(N + 1);

    for(int i = 0;i < N + 1;i++) {
      rnk[sa[i]] = i;
    }
    lcp[0] = 0;
    int h = 0;

    for(int i = 0;i < N + 1;i++) {
      if(rnk[i] + 1 < N + 1) {
        int j = sa[rnk[i] + 1];
        for(; j + h < N + 1 && i + h < N + 1; h++) {
          if(str[j + h] != str[i + h]) break;
        }
        lcp[rnk[i] + 1] = h;
        if(h > 0) h--;
      }
    }
  }

  void build_segment() {
    seg_n = 1;
    while(seg_n < N + 1) seg_n <<= 1;
    seg.resize(seg_n * 2, 1e9);
    for(int i = 0;i + 1 < N + 1;i++) {
      seg[i + seg_n - 1] = lcp[i + 1];
    }
    for(int i = seg_n - 1; i --> 0;) {
      seg[i] = std::min(seg[(i << 1) + 1], seg[(i << 1) + 2]);
    }
  }

  int compare(const std::vector<T>& t, int pos) {
    int i = 0;
    while(pos + i < str.size() && i < t.size()) {
      if(str[pos + i] < t[i]) {
        return 1;
      }
      else if(str[pos + i] > t[i]) {
        return -1;
      }
      i++;
    }
    if(i == t.size()) return 0;
    else return 1;
  }
  

  // fast!
  bool contain(const std::vector<T>& t) {
    int L = 0;
    int R = sa.size();
    while(R - L > 1) {
      int M = (L + R) >> 1;
      if(compare(t, sa[M]) >= 0) {
        L = M;
      }
      else {
        R = M;
      }
    }
    return compare(t, sa[L]) == 0;
  }
};
