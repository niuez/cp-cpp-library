#include <utility>
#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>

struct avl_tree_array {
  using value_type = long long;
  using size_type = std::size_t;
  using height_type = std::int_least32_t;
  using node_index = std::int_least32_t;

  struct node;

  static struct node n[501010];
  static size_type ni;

  struct node {
    value_type val;
    size_type s;
    height_type h;
    node_index c[2];
    value_type f;
    node() : f(0) {}
    node& operator[](size_type d) { return n[c[d]]; }
  };

  static node_index new_node(value_type v) {
    node_index i = ni++;
    n[i].val = v;
    fix(i);
    return i;
  }

  static void fix(node_index i) {
    n[i].s = n[i][0].s + 1 + n[i][1].s;
    n[i].h = std::max(n[i][0].h, n[i][1].h) + 1;
    n[i].f = n[i][0].f + n[i].val + n[i][1].f;
  }

  static void rotate(node_index& x, size_type dir) {
    node_index y = n[x].c[1 ^ dir];
    n[x].c[1 ^ dir] = n[y].c[dir];
    n[y].c[dir] = x;
    fix(x);
    fix(y);
    x = y;
  }

  static void balance(node_index& i) {
    fix(i);
    if(n[i][0].h - n[i][1].h == 2) {
      if(n[i][0][0].h - n[i][0][1].h == -1) {
        rotate(n[i].c[0], 0);
      }
      rotate(i, 1);
    }
    else if(n[i][0].h - n[i][1].h == -2) {
      if(n[i][1][0].h - n[i][1][1].h == 1) {
        rotate(n[i].c[1], 1);
      }
      rotate(i, 0);
    }
  }

  static std::pair<node_index, node_index> deepest_node(node_index i, size_type dir) {
    node_index par = -1;
    while(n[i].c[dir]) {
      std::swap(par, n[i].c[dir]);
      std::swap(par, i);
    }
    node_index ln = n[i].c[dir ^ 1];
    n[i].c[dir ^ 1] = 0;
    node_index dn = i;
    fix(dn);
    i = ln;
    while(std::swap(par, i), i > 0) {
      std::swap(par, n[i].c[dir]);
      balance(i);
    }
    return std::pair<node_index, node_index>(par, dn);
  }

  static node_index merge_dir(node_index l, node_index root, node_index r, size_type dir) {
    node_index par = -1;
    while(std::abs(n[l].h - n[r].h) > 1) {
      std::swap(par, n[l].c[dir]);
      std::swap(par, l);
    }
    n[root].c[dir ^ 1] = l;
    n[root].c[dir] = r;
    fix(root);
    l = root;
    while(std::swap(par, l), l > 0) {
      std::swap(par, n[l].c[dir]);
      balance(l);
    }
    return par;
  }

  static node_index merge(node_index l, node_index r) {
    if(!l) return r;
    else if(!r) return l;
    else if(n[l].h >= n[r].h) {
      auto p = deepest_node(r, 0);
      return merge_dir(l, p.second, p.first, 1);
    }
    else {
      auto p = deepest_node(l, 1);
      return merge_dir(r, p.second, p.first, 0);
    }
  }

  static std::pair<node_index, node_index> split(node_index root, size_type pos) {
    if(pos == n[root].s) return { root, 0 };
    node_index i = root;
    node_index par = -1;
    while(i > 0 && pos != n[i][0].s) {
      if(pos < n[i][0].s) {
        std::swap(par, n[i].c[0]);
      }
      else {
        pos -= n[i][0].s + 1;
        std::swap(par, n[i].c[1]);
      }
      std::swap(par, i);
    }

    node_index l = n[i].c[0];
    node_index r = merge_dir(n[i].c[1], i, 0, 0);

    while(std::swap(par, i), i > 0) {
      if(n[i].c[0] == -1 || n[i].h < n[i][0].h) {
        par = n[i].c[0];
        r = merge_dir(n[i].c[1], i, r, 0);
      }
      else {
        par = n[i].c[1];
        l = merge_dir(n[i].c[0], i, l, 1);
      }
    }
    return { l, r };
  }

  static node_index at(node_index i, size_type pos) {
    while(pos != n[i][0].s) {
      if(pos < n[i][0].s) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].s + 1;
        i = n[i].c[1];
      }
    }
    return i;
  }

  static void set(node_index i, size_type pos, value_type v) {
    node_index par = -1;
    while(pos != n[i][0].s) {
      if(pos < n[i][0].s) {
        std::swap(par, n[i].c[0]);
      }
      else {
        pos -= n[i][0].s + 1;
        std::swap(par, n[i].c[1]);
      }
      std::swap(par, i);
    }
    n[i].val = v;
    fix(i);

    while(std::swap(par, i), i > 0) {
      if(n[i].c[0] == -1 || n[i].h < n[i][0].h) {
        std::swap(par, n[i].c[0]);
      }
      else {
        std::swap(par, n[i].c[1]);
      }
      fix(i);
    }
  }

  static void debug(node_index i, std::string s) {
    if(i == 0) {
      std::cout << s << 0 << std::endl;
      return;
    }
    std::cout << s << i << " = " << n[i].val << " = " << n[i].h << std::endl;
    debug(n[i].c[0], s + "  ");
    debug(n[i].c[1], s + "  ");
  }

public:

  node_index root;
  avl_tree_array(): root(0) {}
  avl_tree_array(value_type val): root(new_node(val)) {}

  avl_tree_array& merge(avl_tree_array right) {
    root = merge(this->root, right.root);
    return *this;
  }
  avl_tree_array split(size_type i) {
    auto p = split(root, i);
    avl_tree_array avl;
    avl.root = p.second;
    root = p.first;
    return avl;
  }

  void set(size_type i, value_type v) {
    set(root, i, v);
  }

  const value_type& at(size_type i) {
    return n[at(root, i)].val;
  }

  value_type fold() {
    return n[root].f;
  }

  size_type size() {
    return n[root].s;
  }

  void debug() {
    debug(root, "");
  }
};

avl_tree_array::node avl_tree_array::n[501010];
avl_tree_array::size_type avl_tree_array::ni = 1;

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
  using niu::fout;
  using niu::fin;

  int N, Q; fin >> N >> Q;
  avl_tree_array arr;
  rep(i,0,N) {
    i64 a;
    fin >> a;
    arr.merge(avl_tree_array(a));
  }
  i64 q, a, b;
  while(Q--) {
    fin >> q >> a >> b;
    if(q == 0) {
      arr.set(a, b + arr.at(a));
    }
    else {
      auto Q = arr.split(b);
      auto P = arr.split(a);
      fout << P.fold() << '\n';
      arr.merge(P).merge(Q);
    }
  }
}
