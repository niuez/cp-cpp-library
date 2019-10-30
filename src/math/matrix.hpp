#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define all(x) x.begin(),x.end()
#define let auto const
 
const i64 mod = 1e9 + 7;
 
template<const i64 N, const i64 M>
struct matrix {
  using T = i64;
  vector<vector<T>> mat;
  matrix(T init = T()): mat(N, vector<T>(M, init)) {}
  vector<T>& operator[](i64 i) { return mat[i]; }
  const vector<T>& operator[](i64 i) const { return mat[i]; }
  static matrix<N, N> E() {
    matrix<N, N> mat(0);
    rep(i,0, N) mat[i][i] = 1;
    return mat;
  }
};
template<const i64 L, const i64 M, const i64 N>
matrix<L, N> operator*(const matrix<L, M>& lm, const matrix<M, N>& rm) {
  matrix<L, N> ret;
  rep(i, 0, L) rep(j, 0, M) rep(k, 0, N) (ret[i][k] += lm[i][j] * rm[j][k]) %= mod;
  return ret;
}
 
template<const i64 N>
matrix<N, N> matrix_pow(matrix<N, N> mat, i64 r) {
  matrix<N, N> ret = matrix<N, N>::E();
  while(r > 0) {
    if(r & 1) ret = ret * mat;
    mat = mat * mat;
    r >>= 1;
  }
  return ret;
}
