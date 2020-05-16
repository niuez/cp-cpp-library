#include <bitset>
#include <array>
#include <vector>

template<const int H, const int W>
struct bit_matrix {
  std::array<std::bitset<W>, H> m;

  bit_matrix(){} 

  std::bitset<W>& operator[](int i) { return m[i]; }
  const std::bitset<W>& operator[](int i) const { return m[i]; }

  template<const int X>
  bit_matrix<H, X> operator*(const bit_matrix<W, X>& r) const {
    bit_matrix<H, X> ans;
    bit_matrix<X, W> tr;
    for(int i = 0;i < W;i++) for(int j = 0;j < X;j++) {
      if(r[i][j]) tr[j].set(i);
    }
    for(int i = 0;i < H;i++) for(int j = 0;j < X;j++) {
      ans[i][j] = (m[i] & tr[j]).count() & 1;
    }
    return ans;
  }
};

template<const int H, const int W>
std::vector<int> gauss_jordan(bit_matrix<H, W>& A, bool is_extended = false) {
  int rank = 0;
  std::vector<int> fst;
  for(int j = 0; j < W; j++) {
    if(is_extended && j + 1 == W) break;
    int pivot = -1;
    for(int i = rank; i < H; i++) {
      if(A[i].test(j)) {
        pivot = i;
        break;
      }
    }
    if(pivot == -1) continue;
    std::swap(A[rank], A[pivot]);
    for(int i = 0;i < H; i++) {
      if(i != rank && A[i][j]) {
        A[i] ^= A[rank];
      }
    }
    fst.push_back(j);
    rank++;
  }
  return fst;
}

const int N_MAX = 200;
const int W_MAX = N_MAX * N_MAX;

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define rep(i,s,e) for(i64 (i) = (s);(i) < (e);(i)++)
#define all(x) x.begin(),x.end()

template<class T>
static inline std::vector<T> ndvec(size_t&& n, T val) noexcept {
  return std::vector<T>(n, std::forward<T>(val));
}

template<class... Tail>
static inline auto ndvec(size_t&& n, Tail&&... tail) noexcept {
  return std::vector<decltype(ndvec(std::forward<Tail>(tail)...))>(n, ndvec(std::forward<Tail>(tail)...));
}

template<class T, class Cond>
struct chain {
  Cond cond; chain(Cond cond) : cond(cond) {}
  bool operator()(T& a, const T& b) const {
    if(cond(a, b)) { a = b; return true; }
    return false;
  }
};
template<class T, class Cond>
chain<T, Cond> make_chain(Cond cond) { return chain<T, Cond>(cond); }

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  int N, M;
  std::cin >> N >> M;

  std::vector<std::bitset<W_MAX>> A(M);

  rep(ai,0,M) {
    rep(i,0,N) {
      string s;
      cin >> s;
      rep(j,0,N) {
        if(s[j] == '#') {
          A[ai].set(i * N + j);
        }
      }
    }
  }

  bit_matrix<N_MAX * 6, W_MAX> trans;

  int cnt = 0;
  rep(s,0,N) {
    rep(i,0,N) {
      trans[cnt][s * N + i] = 1;
    }
    cnt++;
  }
  rep(s,0,N) {
    rep(i,0,N) {
      trans[cnt][i * N + s] = 1;
    }
    cnt++;
  }
  rep(k, 0, N + N - 1) {
    // k = i + j
    rep(i,0,N) {
      int j = k - i;
      if(j >= N || j < 0) continue;
      trans[cnt][i * N + j] = 1;
    }
    cnt++;
  }
  rep(k,-N + 1, N) {
    // k = i - j
    rep(i,0,N) {
      int j = - k + i;
      if(j >= N || j < 0) continue;
      trans[cnt][i * N + j] = 1;
    }
    cnt++;
  }

  auto fst = gauss_jordan(trans);

  for(int i = 0; i < fst.size(); i++) {
    int f = fst[i];
    //std::cout << f << " = " << trans[i] << "\n";
    rep(j,0,M) {
      if(A[j][f]) {
        A[j] ^= trans[i];
      }
    }
  }
  rep(i,0,M - 1) {
    rep(j,i + 1,M) {
      std::cout << (A[i] == A[j]);
    }
    std::cout << '\n';
  }
}
