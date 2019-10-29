#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(int (i) = (s);(i) < (e);(i)++)
#define all(x) x.begin(),x.end()
#define let auto const

/*
## calc dp[i][j] = min[i<=k<j](dp[i][k] + dp[k + 1][j]) + f(i,j). takes O(N^2) and Memory O(N^2)
## f is required of Quandrangle Inequality and Monotone on the Lattice Intervals.
## QI ... f(a or b) + f(a and b) >= f(a) + f(b)
## MLI ...if a is partial of b , f(a) <= f(b)
*/
template<typename F>
i64 mongeDP(i64 N, F f) {
  vector<vector<i64>> dp(N, vector<i64>(N, 1e18));
  vector<vector<i64>> k(N, vector<i64>(N));
  rep(i, 0, N) {
    dp[i][i] = 0;
    k[i][i] = i;
  }

  rep(d, 1, N) {
    rep(i, 0, N - d) {
       i64 j = i + d;
       i64 l = k[i][j - 1];
       i64 r = k[i + 1][j];
       rep(s, l, r + 1) {
         if(s + 1 >= N) break;
         if(dp[i][s] + dp[s + 1][j] <= dp[i][j]) {
           dp[i][j] = dp[i][s] + dp[s + 1][j];
           k[i][j] = s;
         }
       }
       dp[i][j] += f(i, j);
    }
  }
  return dp[0][N - 1];
}

int main() {
  i64 N;
  cin >> N;

  vector<i64> W;
  W.push_back(0);
  rep(i,0, N) {
    i64 x;
    cin >> x;
    W.push_back(W.back() + x);
  }
  auto f = [&](i64 i, i64 j) { return W[j + 1] - W[i]; };
  cout << mongeDP<decltype(f)>(N, f) << endl;
}
