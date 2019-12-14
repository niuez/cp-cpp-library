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
