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

template<class Band>
struct sparse_table {
  Band ope(const Band& a, const Band b) { return std::min(a, b); }
  int N;
  std::vector<Band> A;
  std::vector<int> log_t;
  std::vector<std::vector<Band>> table;

  sparse_table() {}
  sparse_table(std::vector<Band> arr) : N(arr.size()), A(arr), log_t(arr.size() + 1) {
    for(int i = 2;i < N + 1;i++) {
      log_t[i] = log_t[i >> 1] + 1;
    }
    table.resize(N, std::vector<Band>(log_t[N] + 1));

    for(int i = 0;i < N;i++) {
      table[i][0] = arr[i];
    }

    for(int k = 1;(1 << k) <= N;k++) {
      for(int i = 0;i + (1 << k) <= N;i++) {
        table[i][k] = ope(table[i][k - 1], table[i + (1 << (k - 1))][k - 1]);
      }
    }
  }
  /* [s, t) */
  Band query(int s, int t) {
    int k = log_t[t - s];
    return ope(table[s][k], table[t - (1 << k)][k]);
  }
};

template<class T>
struct suffix_array_search {
  int N;
  std::vector<T> str;
  std::vector<int> rnk;
  std::vector<int> sa;
  std::vector<int> lcp;
  sparse_table<int> st;
  
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

    st = sparse_table<int>(lcp);
  }

  std::pair<int, int> get_lcp(const std::vector<T>& t, int si, int offset) {
    int i = offset;
    si += offset;
    while(i < t.size() && si < N) {
      if(t[i] != str[si]) {
        return { i, t[i] - str[si] };
      }
      i++;
      si++;
    }
    return { i, 0 };
  }

  std::pair<int, int> search(const std::vector<T>& t) {
    int L = 0;
    int R = N + 1;
    int Llcp = 0;

    while(R - L > 1) {
      int M = (L + R) >> 1;
      int nlcp = st.query(L + 1, M + 1);
      if(Llcp < nlcp) {
        L = M;
      }
      else if(Llcp > nlcp) {
        R = M;
      }
      else {
        auto p = get_lcp(t, sa[M], Llcp);
        if(p.second >= 0) {
          L = M;
          Llcp = p.first;
        }
        else if(p.second < 0) {
          R = M;
        }
      }
    }

    return { Llcp, L };
  }

  struct element {
    int L;
    int R;
    int Llcp;
    int i;
  };

  std::vector<element> parallel_search(const std::vector<std::vector<T>>& t) {
    std::vector<element> now(t.size(), { 0, N + 1, 0, 0 });
    for(int i = 0;i < t.size(); i++)
      now[i].i = i;
    std::vector<element> next;
    std::vector<element> right;

    std::vector<element> ans(t.size());

    while(!now.empty()) {
      int nlcp = -1;
      int L = 0;
      int R = 0;
      int M = 0;
      for(int i = 0;i < now.size(); i++) {
        if(now[i].R - now[i].L <= 1) {
          ans[now[i].i] = std::move(now[i]);
          continue;
        }
        if(nlcp == -1 || now[i - 1].L != now[i].L) {
          L = now[i].L;
          R = now[i].R;
          M = (L + R) >> 1;
          nlcp = st.query(L + 1, M + 1);
          next.insert(std::end(next), std::begin(right), std::end(right));
          right.clear();
        }
        if(now[i].Llcp < nlcp) {
          now[i].L = M;
          next.push_back(std::move(now[i]));
        }
        else if(now[i].Llcp > nlcp) {
          now[i].R = M;
          right.push_back(std::move(now[i]));
        }
        else {
          auto p = get_lcp(t[now[i].i], sa[M], now[i].Llcp);
          if(p.second >= 0) {
            now[i].L = M;
            now[i].Llcp = p.first;
            next.push_back(std::move(now[i]));
          }
          else if(p.second < 0) {
            now[i].R = M;
            right.push_back(std::move(now[i]));
          }
        }
      }

      next.insert(std::end(next), std::begin(right), std::end(right));
      right.clear();

      std::swap(next, now);
      next.clear();
    }

    return ans;
  }
};
