#include <vector>
using namespace std;
using i64 = long long;

template<class T>
struct matrix {
  int N, M;
  vector<vector<T>> mat;
  matrix(int N, int M, T init = T::zero()): N(N), M(M), mat(N, vector<T>(M, init)) {}
  vector<T>& operator[](i64 i) { return mat[i]; }
  const vector<T>& operator[](i64 i) const { return mat[i]; }
  static matrix<T> E(int N) {
    matrix<T> mat(N, N, T::zero());
    for(int i = 0; i < N; i++) {
      mat[i][i] = T::one();
    }
    return mat;
  }
};

template<class T>
matrix<T> operator+(const matrix<T>& lm, const matrix<T>& rm) {
  matrix ret(lm.N, lm.M);
  for(int i = 0; i < lm.N; i++) {
    for(int j = 0; j< lm.M; j++) {
      ret[i][j] = lm[i][j] + rm[i][j];
    }
  }
  return ret;
}
template<class T>
matrix<T> operator*(const matrix<T>& lm, const matrix<T>& rm) {
  matrix<T> ret(lm.N, rm.M);
  for(int i = 0; i < lm.N; i++) {
    for(int j = 0; j < lm.M; j++) {
      for(int k = 0; k < rm.M; k++) {
        ret[i][k] = ret[i][k] + lm[i][j] * rm[j][k];
      }
    }
  }
  return ret;
}

template<class T>
matrix<T> operator*(const matrix<T>& lm, const T& r) {
  matrix<T> ret(lm.N, lm.M);
  for(int i = 0; i < lm.N; i++) {
    for(int j = 0; j< lm.M; j++) {
      ret[i][j] = lm[i][j] * r;
    }
  }
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
