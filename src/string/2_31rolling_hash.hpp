#include <vector>
using u64 = unsigned long long;

struct hash_bases {
  static const u64 MOD = (1ull << 61) - 1;
  const int N;
  std::vector<std::vector<u64>> bases;
  
  inline static u64 mod(u64 x) {
    x = (x >> 61) + (x & MOD);
    if(x >= MOD) {
      x -= MOD;
    }
    return x;
  }

  inline static u64 mul(u64 a, u64 b) {
    u64 ac = a >> 31;
        a  = a & ((1ull << 31) - 1);
    u64 bc = b >> 31;
        b  = b & ((1ull << 31) - 1);
    u64 x = ac * b + bc * a;;
    x = ac * bc * 2 + (x >> 30) + ((x & ((1ull << 30) - 1)) << 31) + a * b;
    return mod(x);
  }
  
  hash_bases(std::vector<u64> bs, int M) : N(bs.size()), bases(M, std::vector<u64>(N, 1)) {
    for(int i = 1; i < M; i++) {
      for(int j = 0;j < N; j++){
        bases[i][j] = mul(bases[i - 1][j], bs[j]);
      }
    }
  }

  u64 operator()(int i, int r) {
    return bases[r][i];
  }
} bases(std::vector<u64> { 9973, 10007 }, 202020);

struct rhash {
  static const u64 MOD = hash_bases::MOD;
  std::vector<u64> h;
  u64 len;

  rhash() : h(bases.N), len(0) {}
  rhash(u64 x): h(bases.N), len(1) {
    x = hash_bases::mod(x);
    for(int i = 0; i < bases.N; i++) {
      h[i] = x;
    }
  }

  rhash next(u64 r) const {
    rhash ans;
    for(int i = 0;i < bases.N; i++) { 
      ans.h[i] = hash_bases::mul(h[i], bases(i, r));
    }
    ans.len = len + r;
    return ans;
  }

  rhash& operator+=(const rhash& r) {
    for(int i = 0;i < bases.N; i++) {
      h[i] += r.h[i];
      if(h[i] >= MOD) h[i] -= MOD;
    }
    len = std::max(len, r.len);
    return (*this);
  }

  rhash& operator-=(const rhash& r) {
    for(int i = 0;i < bases.N; i++) {
      if(h[i] < r.h[i]) {
        h[i] = h[i] + MOD - r.h[i];
      }
      else {
        h[i] = h[i] - r.h[i];
      }
    }
    return (*this);
  }

  rhash operator+(const rhash& r) const { return rhash(*this) += r; }
  rhash operator-(const rhash& r) const { return rhash(*this) -= r; }
  bool operator==(const rhash& r) const {
    bool OK = true;
    for(int i = 0;i < bases.N; i++){
      OK &= h[i] == r.h[i];
    }
    return OK;
  }
};
