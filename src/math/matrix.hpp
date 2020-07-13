#include <bits/stdc++.h>
using namespace std;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)

template<class T>
struct matrix {
  int N, M;
  vector<vector<T>> mat;
  matrix(int N, int M, T init = T()): N(N), M(M), mat(N, vector<T>(M, init)) {}
  vector<T>& operator[](i64 i) { return mat[i]; }
  const vector<T>& operator[](i64 i) const { return mat[i]; }
  static matrix<T> E(int N) {
    matrix<T> mat(N, N, 0);
    rep(i,0,N) mat[i][i] = 1;
    return mat;
  }
};
template<class T>
matrix<T> operator*(const matrix<T>& lm, const matrix<T>& rm) {
  matrix<T> ret(lm.N, rm.M);
  rep(i, 0, lm.N) rep(j, 0, lm.M) rep(k, 0, rm.M) (ret[i][k] += lm[i][j] * rm[j][k]);
  return ret;
}
 
template<class T>
matrix<T> matrix_pow(matrix<T> mat, i64 r) {
  matrix<T> ret = matrix<T>::E(mat.N);
  while(r > 0) {
    if(r & 1) ret = ret * mat;
    mat = mat * mat;
    r >>= 1;
  }
  return ret;
}
