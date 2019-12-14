#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct sparse_table {
  using Band = i64;
  Band ope(const Band& a, const Band b) { return min(a, b); }

  i64 N;
  vector<Band> A;
  vector<i64> log_t;
  vector<vector<Band>> table;

  sparse_table(vector<Band> arr) : N(arr.size()), A(arr), log_t(arr.size() + 1) {
    for(int i = 2;i < N + 1;i++) {
      log_t[i] = log_t[i >> 1] + 1;
    }
    table.resize(N, vector<Band>(log_t[N] + 1));

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
  Band query(i64 s, i64 t) {
    i64 k = log_t[t - s];
    return ope(table[s][k], table[t - (1 << k)][k]);
  }
};
