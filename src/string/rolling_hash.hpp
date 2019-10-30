#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

const i64 rhmod = 1e9 + 7;
const i64 BASE_C = 2;
const i64 base[] = {9973, 10007};

struct rolling_hash {
  i64 n;
  vector<i64> hs[BASE_C], pw[BASE_C];
  
  template<class Array>
  rolling_hash(const Array& arr) {
    n = arr.size();
    for(int i = 0;i < BASE_C;i++) {
      hs[i].assign(n + 1, 0);
      pw[i].assign(n + 1, 0);
      hs[i][0] = 0;
      pw[i][0] = 1;
      for(int j = 0;j < n;j++) {
        pw[i][j + 1] = pw[i][j] * base[i] % rhmod;
        hs[i][j + 1] = (hs[i][j] * base[i] + arr[j]) % rhmod;
      }
    }
  }

  pair<i64, i64> hash(i64 l, i64 r) {
    return {
      ((hs[0][r + 1] - hs[0][l] * pw[0][r + 1 - l]) % rhmod + rhmod) % rhmod,
      ((hs[1][r + 1] - hs[1][l] * pw[1][r + 1 - l]) % rhmod + rhmod) % rhmod,
    };
  }
};
