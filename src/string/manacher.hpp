#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

/*
 * 文字 i を中心とする最長の回文の半径 = (全長 + 1) / 2
 */

vector<i64> manacher(string S) {
  vector<i64> R(S.size());
  int i = 0, j = 0;
  while (i < S.size()) {
    while (i-j >= 0 && i+j < S.size() && S[i-j] == S[i+j]) ++j;
    R[i] = j;
    int k = 1;
    while (i-k >= 0 && i+k < S.size() && k+R[i-k] < j) R[i+k] = R[i-k], ++k;
    i += k; j -= k;
  }
  return R;
}
