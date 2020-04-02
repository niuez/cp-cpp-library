#include <memory>
#include <array>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <bitset>

using std::cout;
using std::endl;


struct dynamic_bitvector {


  struct node;
  using size_type = std::size_t;
  using height_type = std::int_least32_t;
  using node_index = std::size_t;
  using bits_type = std::uint64_t;
  using children_type = std::array<node_index, 2>;


  const static size_type bit_limit = 32;
  const static size_type lg = 6;
  const static size_type lgmask = 63;
  //const static size_type bit_limit = 2;
  //const static size_type lg = 1;
  //const static size_type lgmask = 1;

  /* struct bitvector_builder {
    const static size_type bit_size = bit_limit * 2;
    const size_type len;
    std::vector<bits_type> bits;
    bitvector_builder(size_type len): len(len), bits((len >> lg) + !!(len & lgmask)) {}
    void set(size_type i) { bits[i >> lg] |= (bits_type(1) << (i & lgmask)); }
    dynamic_bitvector build() const {
      return dynamic_bitvector(bits, len);
    }
  }; */

  inline static size_type bits_popcount(bits_type bits) {
    return __builtin_popcountll(bits);
  }
  inline static bits_type bitmask(size_type i) {
    return (bits_type(1) << i) - 1;
  }
  static std::vector<struct node> n;

  struct node {
    size_type sz;
    bits_type bits;
    height_type he;
    std::array<node_index, 2> ch;

    static node_index new_section(node_index left, node_index right) {
      //node* n = new node(section_t { 0, children_type { std::move(left), std::move(right) } });
      node_index i = n.size();
      n.push_back(node());
      n[i].ch[0] = left;
      n[i].ch[1] = right;
      fix(i);
      return i;
    }

    static node_index new_leaf(bits_type bits, size_type sz) {
      //node* n = new node(leaf_t { bits });
      node_index i = n.size();
      n.push_back(node());
      n[i].bits = bits;
      n[i].sz = sz;
      return i;
    }
    
    node(): sz(0), bits(0), he(0), ch({ 0, 0 }) {}

    inline node& operator[](int d) { return n[ch[d]]; }

    inline size_type popcnt() const {
      return he ? bits : bits_popcount(bits);
    }
  };

  static inline void fix(node_index i) {
    //assert(!is_leaf);
    n[i].sz = n[i][0].sz + n[i][1].sz;
    n[i].bits = n[i][0].popcnt() + n[i][1].popcnt();
    n[i].he = std::max(n[i][0].he, n[i][1].he) + 1;
  }

  static void rotate(node_index& x, size_type dir) {
    node_index y = n[x].ch[1 ^ dir];
    n[x].ch[1 ^ dir] = n[y].ch[dir];
    n[y].ch[dir] = x;
    fix(x);
    fix(y);
    x = y;
  }

  static void balance(node_index& i) {
    fix(i);
    //assert(std::abs(node->diff()) <= 2);
    if(n[i][0].he - n[i][1].he == 2) {
      if(n[i][0][0].he - n[i][0][1].he == -1) {
        rotate(n[i].ch[0], 0);
      }
      rotate(i, 1);
    }
    else if(n[i][0].he - n[i][1].he == -2) {
      if(n[i][1][0].he - n[i][1][1].he == 1) {
        rotate(n[i].ch[1], 1);
      }
      rotate(i, 0);
    }
  }

  static inline void insert_bit(node_index i, size_type pos, bool bit) {
    n[i].sz++;
    n[i].bits = (n[i].bits & bitmask(pos)) | (bits_type(bit) << pos) | ((n[i].bits >> pos) << (pos + 1));
  }

  static void insert(node_index& root, size_type pos, bool bit) {
    //cout << "insert" << root << endl;
    node_index i = root;
    node_index par = 0;
    while(n[i].he) {
      //cout << i << endl;
      if(pos < n[i][0].sz) {
        std::swap(par, n[i].ch[0]);
      }
      else {
        pos -= n[i][0].sz;
        std::swap(par, n[i].ch[1]);
      }
      std::swap(par, i);
    }
    if(n[i].sz == 2 * bit_limit) {
      bits_type l, r;
      l = n[i].bits & bitmask(bit_limit);
      r = n[i].bits >> bit_limit;
      node_index nl = node::new_leaf(l, bit_limit);
      node_index nr = node::new_leaf(r, bit_limit);
      n[i].ch[0] = nl;
      n[i].ch[1] = nr;
      n[i].bits = n[i].popcnt() + bit;
      n[i].sz++;
      n[i].he = 1;
      if(pos < bit_limit) insert_bit(nl, pos, bit);
      else                insert_bit(nr, pos - bit_limit, bit);
      //cout << "size = " << n[i].sz << " : " << n[nl].sz << " " << n[nr].sz << endl;
    }
    else {
      insert_bit(i, pos, bit);
    }

    while(std::swap(par, i), i > 0) {
      //cout << i << " " << par << endl;
      if(n[i].ch[0] == 0 || n[i].he < n[i][0].he) {
        std::swap(par, n[i].ch[0]);
      }
      else {
        std::swap(par, n[i].ch[1]);
      }
      balance(i);
    }
    root = par;
  }

  static bool at_(node_index i, size_type pos) {
    while(n[i].he) {
      if(pos < n[i][0].sz) {
        i = n[i].ch[0];
      }
      else {
        pos -= n[i][0].sz;
        i = n[i].ch[1];
      }
    }
    return !!(1 & (n[i].bits >> pos));
  }

  static size_type rank_(node_index i, size_type pos) {
    size_type ans = 0;
    if(pos == n[i].sz) {
      return n[i].popcnt();
    }
    while(n[i].he) {
      if(pos < n[i][0].sz) {
        i = n[i].ch[0];
      }
      else {
        pos -= n[i][0].sz;
        ans += n[i][0].popcnt();
        i = n[i].ch[1];
      }
    }
    return ans + bits_popcount(n[i].bits & bitmask(pos));
  }


  node_index root;

  dynamic_bitvector(): root(node::new_leaf(0, 0)) {}

  void insert(size_type pos, bool bit) {
    insert(root, pos, bit);
  }
  bool at(size_type pos) const {
    return at_(root, pos);
  }
  size_type rank(size_type pos) const {
    return rank_(root, pos);
  }
  size_type rank(size_type pos, bool bit) const {
    if(bit) {
      return rank_(root, pos);
    }
    else {
      return pos - rank_(root, pos);
    }
  }
  size_type size() const {
    return n[root].sz;
  }

  void debug_tree(node_index i, std::string d) const {
    if(n[i].he == 0) {
      std::cout << d << "leaf " << n[i].sz << " " << n[i].popcnt() << " -----------" << std::endl;
      std::cout << d << std::bitset<bit_limit * 2>(n[i].bits) << std::endl;
      std::cout << d << "-------------" << std::endl;
    }
    else {
      std::cout << d << "node " << n[i].sz << " " << n[i].popcnt() << " -----------" << std::endl;
      cout << i << ": " << n[i].popcnt() << " = " << n[i][0].popcnt() << " + " << n[i][1].popcnt() << endl;
      assert(n[i].popcnt() == n[i][0].popcnt() + n[i][1].popcnt());
      std::cout << d << "left" << std::endl;
      debug_tree(n[i].ch[0], d + "   ");
      std::cout << d << "--------------" << std::endl;
      std::cout << d << "right" << std::endl;
      debug_tree(n[i].ch[1], d + "   ");
      std::cout << d << "--------------" << std::endl;
    }
  }

  void debug_tree() const {
    debug_tree(root, "");
  }
};

std::vector<dynamic_bitvector::node> dynamic_bitvector::n(1);

#include <vector>
struct dynamic_wavelet_matrix {
  using Integer = long long;
  using integer_type = Integer;
  using size_type = std::size_t;


  const size_type depth;
  const size_type len;
  std::vector<dynamic_bitvector> mat;
  std::vector<size_type> spl;

public:

  dynamic_wavelet_matrix(size_type de)
    : depth(de),
      len(0),
      mat(de),
      spl(std::vector<size_type>(depth, 0)) {}

  void insert(size_type i, integer_type x) {
    for(size_type d = depth; d --> 0;) {
      size_type k = ((x >> d) & 1);
      mat[d].insert(i, k);
      i = mat[d].rank(i, k) + spl[d] * k;
      if(k == 0) spl[d]++;
    }
  }

  integer_type at(size_type i) const {
    integer_type x = static_cast<integer_type>(0);
    for(size_type d = depth; d-- > 0;) {
      size_type k = mat[d].at(i);
      x |= (static_cast<integer_type>(k) << d);
      i = mat[d].rank(i, k) + spl[d] * k;
    }
    return x;
  }

  // counting elements that equal to x in range [left, right)
  size_type rank_x(size_type left, size_type right, integer_type x) const {
    for(size_type d = depth; d-- > 0;) {
      size_type k = ((x >> d) & 1);
      left = mat[d].rank(left, k) + spl[d] * k;
      right = mat[d].rank(right, k) + spl[d] * k;
    }
    return right - left;
  }

  // sorted(arr[left..right])[i]
  integer_type quantile(size_type left, size_type right, size_type i) const {
    integer_type x = static_cast<integer_type>(0);
    for(size_type d = depth; d-- > 0;) {
      size_type l = mat[d].rank(left, 1);
      size_type r = mat[d].rank(right, 1);
      size_type cnt = (right - left) - (r - l);
      size_type k = (i < cnt) ? 0 : 1;
      if(k == 0) {
        left -= l;
        right -= r;
      }
      else {
        x |= (1 << d);
        i -= cnt;
        left = l + spl[d];
        right = r + spl[d];
      }
    }
    return x;
  }

  struct rank_result {
    size_type le;
    size_type eq;
    size_type mo;
  };

  // couting elements that less than x, equal to x, and more than x in range [left, right)
  rank_result rank_less_eq_more(size_type left, size_type right, integer_type x) const {
    size_type le = 0, mo = 0;
    for(size_type d = depth; d --> 0;) {
      size_type k = (x >> d) & 1;
      size_type l = mat[d].rank(left, 1);
      size_type r = mat[d].rank(right, 1);
      if(k == 0) {
        mo += r - l;
        left -= l;
        right -= r;
      }
      else {
        le += (right - left) - (r - l);
        left = l + spl[d];
        right = r + spl[d];
      }
    }
    return rank_result { le, right - left, mo };
  }

  size_type rangefreq(size_type left, size_type right, integer_type x, integer_type y, integer_type l, size_type d) const {
    integer_type r = l + (1 << d);
    if(x <= l && r <= y) {
      return right - left;
    }
    else if(y <= l || r <= x) {
      return 0;
    }
    else {
      d--;
      size_type lr = mat[d].rank(left, 1);
      size_type rr = mat[d].rank(right, 1);
      return
        rangefreq(left - lr, right - rr, x, y, l, d) +
        rangefreq(lr + spl[d], rr + spl[d], x, y, l + (1 << d), d);
    }
  }

  size_type rangefreq(size_type left, size_type right, integer_type x, integer_type y) const {
    return rangefreq(left, right, x, y, 0, depth);
  }

  size_type rangefreq_count(size_type left, size_type right, integer_type x, integer_type y) const {
    auto p = rank_less_eq_more(left, right, x);
    auto q = rank_less_eq_more(left, right, y);
    return p.eq + p.mo - q.eq - q.mo;
  }
};

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

int solve() {
  for(i64 N = 2100; ; N += 500) {
    dynamic_bitvector b;
    vector<bool> vec;
    rep(i,0,N) {
      i64 j = rand() % (vec.size() + 1);
      b.insert(j, i % 2);
      vec.insert(vec.begin() + j, i % 2);
      //b.debug_tree();
    }
    //b.debug_tree();
    rep(i,0,N) {
      assert(b.at(i) == vec[i]);
    }
    i64 sum = 0;
    rep(i,0,N) {
      i64 ans = b.rank(i);
      cout << i << " = " << sum << " : " << ans << endl;
      assert(sum == ans);
      sum += vec.at(i);
    }
    i64 ans = b.rank(N);
    cout << N << " = " << sum << " : " << ans << endl;
    assert(sum == ans);
  }
  return 0;
}

int main() {
  cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  i64 N, Q;
  cin >> N >> Q;
  
  dynamic_wavelet_matrix wm(30);

  rep(i,0,N) {
    i64 a;
    cin >> a;
    wm.insert(i, a);
  }

  while(Q--) {
    i64 l, r, k;
    cin >> l >> r >> k;
    cout << wm.quantile(l, r, k) << "\n";
  }
  return 0;
}
