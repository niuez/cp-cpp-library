# Code

```cpp
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define rev(i,s,e) for(i64 (i) = (s);(i) --> (e);)
#define all(x) x.begin(),x.end()
 
template<class T, const i64 N, const i64 M>
struct matrix {
  vector<vector<T>> mat;
  matrix(T init = T()): mat(N, vector<T>(M, init)) {}
  vector<T>& operator[](i64 i) { return mat[i]; }
  const vector<T>& operator[](i64 i) const { return mat[i]; }
  static matrix<T, N, N> E() {
    matrix<T, N, N> mat(0);
    rep(i,0,N) mat[i][i] = 1;
    return mat;
  }
};
template<class T, const i64 L, const i64 M, const i64 N>
matrix<T, L, N> operator*(const matrix<T, L, M>& lm, const matrix<T, M, N>& rm) {
  matrix<T, L, N> ret;
  rep(i, 0, L) rep(j, 0, M) rep(k, 0, N) (ret[i][k] += lm[i][j] * rm[j][k]);
  return ret;
}
 
template<class T, const i64 N>
matrix<T, N, N> matrix_pow(matrix<T, N, N> mat, i64 r) {
  matrix<T, N, N> ret = matrix<T, N, N>::E();
  while(r > 0) {
    if(r & 1) ret = ret * mat;
    mat = mat * mat;
    r >>= 1;
  }
  return ret;
}
```
