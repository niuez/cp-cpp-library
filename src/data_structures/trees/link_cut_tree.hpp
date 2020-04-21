include <cstdint>
#include <utility>
#include <string>
#include <iostream>

using i64 = long long;

namespace lctree {

  struct R {
    int a;
    R(): a(0) {}
    R(int a): a(a) {}
  };
  struct V {
    int a;
    V(): a(0) {}
    V(int a): a(a) {}
  };
  inline V compress(const V& a, const V& b) { return V(a.a + b.a); }
  inline V rake_merge(const V& a, const R& b) { return V(a.a + b.a); }
  inline V reverse(const V& a) { return a; }
  inline void rake_plus(R& a, const V& b) { a.a += b.a; }
  inline void rake_minus(R& a, const V& b) { a.a -= b.a; }

  struct node;
  extern struct node n[505050];
  extern int ni;

  using node_index = std::uint_least32_t;
  using size_type = std::size_t;

  struct node {
    node_index c[3];
    V v; V f; R r;
    bool rev;
    node(): rev(false) { c[0] = c[1] = c[2] = 0; }
    node& operator[](int d) { return n[c[d]]; }
  };

  inline node_index new_node(V v) { n[ni].v = v; n[ni].f = v; return ni++; }
  inline void reverse(node_index i) {
    n[i].v = reverse(n[i].v);
    n[i].f = reverse(n[i].f);
    n[i].rev ^= true;
  }
  inline void push(node_index i) {
    if(n[i].rev) {
      std::swap(n[i].c[0], n[i].c[1]);
      if(n[i].c[0]) reverse(n[i].c[0]);
      if(n[i].c[1]) reverse(n[i].c[1]);
      n[i].rev = false;
    }
  }
  inline void fix(node_index i) {
    push(i);
    n[i].f = compress(compress(n[i][0].f, n[i].v), rake_merge(n[i][1].f, n[i].r));
  }

  inline int child_dir(node_index i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) { return 0; }
      else if(n[i][2].c[1] == i) { return 1; }
    }
    return 3;
  }

  inline void rotate(node_index x, size_type dir) {
    node_index p = n[x].c[2];
    int x_dir = child_dir(x);
    node_index y = n[x].c[dir ^ 1];

    n[n[y][dir].c[2] = x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(x_dir < 2) n[p].c[x_dir] = y;
    fix(n[x].c[dir ^ 1]);
    fix(x);
  }

  void splay(node_index i) {
    push(i);
    int i_dir;
    int j_dir;
    while(child_dir(i) < 2) {
      node_index j = n[i].c[2];
      if(child_dir(j) < 2) {
        node_index k = n[j].c[2];
        push(k), push(j), push(i);
        i_dir = child_dir(i);
        j_dir = child_dir(j);
        if(i_dir == j_dir) rotate(k, j_dir ^ 1), rotate(j, i_dir ^ 1);
        else rotate(j, i_dir ^ 1), rotate(k, j_dir ^ 1);
      }
      else push(j), push(i), rotate(j, child_dir(i) ^ 1);
    }
    fix(i);
  }

  node_index expose(node_index i) {
    node_index right = 0;
    node_index ii = i;
    while(i) {
      splay(i);
      rake_minus(n[i].r, n[right].f);
      rake_plus(n[i].r, n[i][1].f);
      n[i].c[1] = right;
      fix(i);
      right = i;
      i = n[i].c[2];
    }
    splay(ii);
    return ii;
  }

  void link(node_index i, node_index j) {
    if(!i || !j) return;
    expose(i);
    expose(j);
    n[n[j].c[2] = i].c[1] = j;
    fix(i);
  }

  void cut(node_index i) {
    if(!i) return;
    expose(i);
    node_index p = n[i].c[0];
    n[i].c[0] = n[p].c[2] = 0;
    fix(i);
  }

  void evert(node_index i) {
    if(!i) return;
    expose(i);
    reverse(i);
    push(i);
  }

  node n[505050];
  int ni = 1;

  int all_tree(node_index i) {
    expose(i);
    return n[i].f.a;
  }
}

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

#include <cstdio>
 
namespace niu {
  char cur;
  struct FIN {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline char next() { return cur = getc_unlocked(stdin); }
    inline char peek() { return cur; }
    inline void skip() { while(is_blank(next())){} }
#define intin(inttype)  \
    FIN& operator>>(inttype& n) { \
      bool sign = 0; \
      n = 0; \
      skip(); \
      while(!is_blank(peek())) { \
        if(peek() == '-') sign = 1; \
        else n = (n << 1) + (n << 3) + (peek() & 0b1111); \
        next(); \
      } \
      if(sign) n = -n; \
      return *this; \
    }
intin(int)
intin(long long)
  } fin;
 
  char tmp[128];
  struct FOUT {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline void push(char c) { putc_unlocked(c, stdout); }
    FOUT& operator<<(char c) { push(c); return *this; }
    FOUT& operator<<(const char* s) { while(*s) push(*s++); return *this; }
#define intout(inttype) \
    FOUT& operator<<(inttype n) { \
      if(n) { \
        char* p = tmp + 127; bool neg = 0; \
        if(n < 0) neg = 1, n = -n; \
        while(n) *--p = (n % 10) | 0b00110000, n /= 10; \
        if(neg) *--p = '-'; \
        return (*this) << p; \
      } \
      else { \
        push('0'); \
        return *this; \
      } \
    }
intout(int)
intout(long long)
  } fout;
}

int main() {
  using niu::fin;
  using niu::fout;
  i64 N, Q;
  fin >> N;
  vector<vector<int>> vs(N);
  vector<int> co(N);
  for(int i = 0;i < N;i++) {
    lctree::new_node(1);
    int a;
    fin >> a;
    a--;
    co[i] = a;
    vs[a].push_back(i);
  }
  vector<vector<int>> G(N);
  for(int i = 0;i + 1 < N;i++) {
    i64 a, b;
    fin >> a >> b;
    a--;
    b--;
    G[a].push_back(b);
    if(co[a] != co[b])
      G[b].push_back(a);
    lctree::evert(b + 1);
    lctree::link(a + 1, b + 1);
  }
 
  auto func = [&](i64 ans, i64 a, i64 b) {
    i64 A = lctree::all_tree(a);
    ans -= A * (A + 1) / 2;
    lctree::evert(a);
    lctree::cut(b);
    i64 B = lctree::all_tree(a);
    ans += B * (B + 1) / 2;
    i64 C = lctree::all_tree(b);
    ans += C * (C + 1) / 2;
    //std::cout << A << " " << B << " " << C << std::endl;
    return ans;
  };
  for(int i = 0;i < N;i++) {
    i64 ans = (N - vs[i].size()) * ((N - vs[i].size()) + 1) / 2;
    for(auto v: vs[i]) {
      lctree::expose(v + 1);
      lctree::n[v + 1].v.a = 0;
      lctree::fix(v + 1);
    }
    for(auto v: vs[i]) {
      for(auto t: G[v]) {
        ans = func(ans, v + 1, t + 1);
      }
    }
    //cout << ans << endl;
    fout << (N * (N + 1) / 2) - ans << "\n";
    for(auto v: vs[i]) {
      lctree::expose(v + 1);
      lctree::n[v + 1].v.a = 1;
      lctree::fix(v + 1);
    }
    for(auto v: vs[i]) {
      for(auto t: G[v]) {
        lctree::evert(t + 1);
        lctree::link(v + 1, t + 1);
      }
    }
  }
}
 
