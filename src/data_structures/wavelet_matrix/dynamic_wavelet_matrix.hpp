#include <array>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <bitset>

struct dynamic_bitvector {


  struct node;
  using size_type = std::size_t;
  using height_type = long long;
  using node_type = std::unique_ptr<struct node>;
  using node_reference = const node_type&;
  using bits_type = std::uint64_t;
  using children_type = std::array<node_type, 2>;


  const static size_type bit_limit = 32;

  struct bitvector_builder {
    const static size_type bit_size = bit_limit * 2;
    const size_type len;
    std::vector<bits_type> bits;
    bitvector_builder(size_type len): len(len), bits(len / bit_size + !!(len & (bit_size - 1))) {}
    void set(size_type i) { bits[i / bit_size] |= (bits_type(1) << (i & (bit_size - 1))); }
    dynamic_bitvector build() const {
      return dynamic_bitvector(bits, len);
    }
  };

  static size_type bits_popcount(bits_type bits) {
    return __builtin_popcountll(bits);
  }

  struct section_t {
    height_type he;
    children_type ch;
  };

  struct leaf_t {
    bits_type bits;
  };

  union info_t {
    section_t section;
    leaf_t leaf;
    info_t(section_t sec): section(std::move(sec)) {}
    info_t(leaf_t leaf): leaf(leaf) {}
    ~info_t() {}
  };

  struct node {
    const bool is_leaf;
    size_type sz;
    size_type popcnt;
    info_t info;

    static node_type new_section(node_type left, node_type right) {
      node* n = new node(section_t { 0, children_type { std::move(left), std::move(right) } });
      n->fix();
      return node_type(n);
    }

    static node_type new_leaf(bits_type bits, size_type sz, size_type popcnt) {
      node* n = new node(leaf_t { bits });
      n->sz = sz;
      n->popcnt = popcnt;
      return node_type(n); } node(section_t sec): is_leaf(false), info(std::move(sec)) {}
    node(leaf_t leaf): is_leaf(true), info(leaf) {}

    height_type height() const {
      if(is_leaf) return 0;
      else return info.section.he;
    }
    size_type size() const {
      return sz;
    }

    size_type popcount() const {
      return popcnt;
    }

    // operation for only section node

    node_type take(size_type dir) {
      assert(!is_leaf);
      return std::move(info.section.ch[dir]);
    }

    node_type swap(size_type dir, node_type new_node) {
      assert(!is_leaf);
      node_type old_node = take(dir);
      info.section.ch[dir] = std::move(new_node);
      return old_node;
    }

    const node_type& child(size_type dir) const {
      assert(!is_leaf);
      return info.section.ch[dir];
    }

    height_type diff() const {
      assert(!is_leaf);
      return child(0)->height() - child(1)->height();
    }

    void fix() {
      assert(!is_leaf);
      sz = child(0)->size() + child(1)->size();
      popcnt = child(0)->popcount() + child(1)->popcount();
      info.section.he = std::max(child(0)->height(), child(1)->height()) + 1;
    }

    // operation for only leaf node

    bits_type bits() const {
      assert(is_leaf);
      return info.leaf.bits;
    }

    bool at_bits(size_type pos) const {
      assert(is_leaf);
      return (bits() >> pos) & 1;
    }

    size_type rank(size_type pos) const {
      assert(pos <= size());
      return bits_popcount(bits() & ((bits_type(1) << pos) - 1));
    }

    void set_bits(bits_type bits, size_type sz_, size_type popcnt_) {
      assert(is_leaf);
      sz = sz_;
      popcnt = popcnt_;
      info.leaf.bits = bits;
    }

    // [0, pos) and [pos, sz)
    std::pair<bits_type, bits_type> split_bits(size_type pos) const {
      assert(is_leaf);
      assert(pos <= size());
      return std::make_pair(bits() & ((bits_type(1) << pos) - 1), bits() >> pos);
    }

    void insert_bit(size_type pos, bool bit) {
      assert(is_leaf);
      assert(sz < bit_limit * 2);
      bits_type l, r;
      std::tie(l, r) = split_bits(pos);
     // std::cout << "insert " << bit  << " to " << pos << std::endl;
     // std::cout << "insert " << std::bitset<bit_limit * 2>(l) << " " << std::bitset<bit_limit * 2>(r) << std::endl;
     // std::cout << "insert " << std::bitset<bit_limit * 2>(bits()) << " -> " << std::bitset<bit_limit * 2>(l | ((bits_type(bit) << pos)) | (r << (pos + 1))) << std::endl;
      set_bits(l | ((bits_type(bit) << pos)) | (r << (pos + 1)), sz + 1, popcount() + bit);
    } 

    void erase_bit(size_type pos) {
      assert(is_leaf);
      assert(0 < sz);
      bits_type l, r;
      std::tie(l, r) = split_bits(pos);
      size_type pos_bit = r & 1;
      r >>= 1;
      set_bits(l | (r << pos), sz - 1, popcount() - pos_bit);
    }
  };


  static node_type rotate(node_type x, size_type dir) {
    x->fix();
    node_type y = x->take(1 - dir);
    node_type b = y->take(dir);
    x->swap(1 - dir, std::move(b));
    x->fix();
    y->swap(dir, std::move(x));
    y->fix();
    return std::move(y);
  }

  static node_type balance(node_type node) {
    node->fix();
    if(node->diff() == 2) {
      if(node->child(0)->diff() == -1) {
        auto ch = node->take(0);
        node->swap(0, rotate(std::move(ch), 0));
      }
      return rotate(std::move(node), 1);
    }
    else if(node->diff() == -2) {
      if(node->child(1)->diff() == 1) {
        auto ch = node->take(1);
        node->swap(1, rotate(std::move(ch), 1));
      }
      return rotate(std::move(node), 0);
    }
    else return std::move(node);
  }

  static node_type split_node(node_type node) {
    assert(node->is_leaf);
    bits_type l, r;
    std::tie(l, r) = node->split_bits(bit_limit);
    return node::new_section(
        node::new_leaf(l, bit_limit, bits_popcount(l)), node::new_leaf(r, node->size() - bit_limit, bits_popcount(r))
        );
  }

  static node_type insert(node_type node, size_type pos, bool bit) {
    if(node->is_leaf) {
      if(node->size() == 2 * bit_limit) {
        return insert(split_node(std::move(node)), pos, bit);
      }
      else {
        node->insert_bit(pos, bit);
        return node;
      }
    }
    else if(pos < node->child(0)->size()) {
      node->swap(0, insert(node->take(0), pos, bit));
      return balance(std::move(node));
    }
    else {
      node->swap(1, insert(node->take(1), pos - node->child(0)->size(), bit));
      return balance(std::move(node));
    }
  }

  template<const size_type dir>
  static std::tuple<node_type, bits_type, size_type> take_bit(node_type node, size_type len) {
    if(node->is_leaf) {
      if(node->size() < bit_limit / 2 + len) {
        return std::tuple<node_type, bits_type, size_type>{ node_type(nullptr), node->bits(), node->size() };
      }
      else {
        auto p = node->split_bits(dir == 0 ? len : node->size() - len);
        bits_type node_bit = std::get<1 - dir>(p);
        bits_type take = std::get<dir>(p);
        node->set_bits(node_bit, node->size() - len, bits_popcount(node_bit));
        return std::tuple<node_type, bits_type, size_type> { std::move(node), take, len };
      }
    }
    else {
      node_type ch;
      bits_type bits;
      size_type take_len;
      std::tie(ch, bits, take_len) = take_bit<dir>(node->take(dir), len);
      if(ch) {
        node->swap(dir, std::move(ch));
        return std::tuple<node_type, bits_type, size_type>{ balance(std::move(node)), bits, take_len };
      }
      else {
        return std::tuple<node_type, bits_type, size_type> { node->take(1 - dir), bits, take_len };
      }
    }
  }

  static node_type erase(node_type node, size_type pos) {
    if(node->is_leaf) {
      node->erase_bit(pos);
      return node;
    }
    else if(pos < node->child(0)->size()) {
      auto left = erase(node->take(0), pos);
      if(left->size() < bit_limit / 2) {
        assert(left->is_leaf);
        node_type right;
        bits_type bits;
        size_type len;
        std::tie(right, bits, len) = take_bit<0>(node->take(1), bit_limit / 2 - left->size());

        left->set_bits(left->bits() | (bits << left->size()), left->size() + len, left->popcount() + bits_popcount(bits));
        if(right) {
          node->swap(0, std::move(left));
          node->swap(1, std::move(right));
          return balance(std::move(node));
        }
        else {
          return left;
        }
      }
      else {
        node->swap(0, std::move(left));
        return balance(std::move(node));
      }
    }
    else {
      auto right = erase(node->take(1), pos - node->child(0)->size());
      if(right->size() < bit_limit / 2) {
        assert(right->is_leaf);
        node_type left;
        bits_type bits;
        size_type len;
        std::tie(left, bits, len) = take_bit<1>(node->take(0), bit_limit / 2 - right->size());

        right->set_bits((right->bits() << len) | bits, right->size() + len, right->popcount() + bits_popcount(bits));
        if(left) {
          node->swap(0, std::move(left));
          node->swap(1, std::move(right));
          return balance(std::move(node));
        }
        else {
          return right;
        }
      }
      else {
        node->swap(1, std::move(right));
        return balance(std::move(node));
      }
    }
  }

  static node_type merge_dir(node_type dst, node_type src, size_type dir) {
    if(std::abs(dst->height() - src->height()) <= 1) {
      if(dir == 0)
        return node::new_section(std::move(src), std::move(dst));
      else
        return node::new_section(std::move(dst), std::move(src));
    }
    else {
      node_type ch = dst->take(dir);
      assert(ch);
      dst->swap(dir, merge_dir(std::move(ch), std::move(src), dir));
      return balance(std::move(dst));
    }
  }

  static node_type merge(node_type left, node_type right) {
    if(!left) { return std::move(right); }
    else if(!right) { return std::move(left); }
    else if(left->height() >= right->height()) {
      return merge_dir(std::move(left), std::move(right), 1);
    }
    else {
      return merge_dir(std::move(right), std::move(left), 0);
    }
  }

  static node_type build(const std::vector<bits_type>& bits, size_type l, size_type r, size_type len) {
    if(len == 0) node::new_leaf(0, 0, 0);
    //std::cout << "build " << l << " " << r << " " << len << std::endl;
    if(l + 1 >= r) {
      //std::cout << "create leaf" << std::endl;
      //std::cout << "-----------" << std::endl;
      return node::new_leaf(bits[l], len, bits_popcount(bits[l]));
    }
    else {
      size_type m = (l + r) >> 1;
      return merge(build(bits, l, m, (m - l) * 64), build(bits, m, r, len - (m - l) * 64));
    }
  }

  static size_type at(node_reference node, size_type pos) {
    if(node->is_leaf) {
      return node->at_bits(pos);
    }
    else if(pos < node->child(0)->size()) {
      return at(node->child(0), pos);
    }
    else {
      return at(node->child(1), pos - node->child(0)->size());
    }
  }

  static size_type rank(node_reference node, size_type pos) {
    if(node->is_leaf) {
      return node->rank(pos);
    }
    else if(pos == node->size()) {
      return node->popcount();
    }
    else if(pos < node->child(0)->size()) {
      return rank(node->child(0), pos);
    }
    else {
      return node->child(0)->popcount() + rank(node->child(1), pos - node->child(0)->size());
    }
  }


  node_type root;

  dynamic_bitvector(): root(node::new_leaf(0, 0, 0)) {}
  dynamic_bitvector(const std::vector<bits_type>& bits, size_type len): root(build(bits, 0, bits.size(), len)) {}
  void insert(size_type pos, bool bit) {
    root = insert(std::move(root), pos, bit);
  }
  void erase(size_type pos) {
    root = erase(std::move(root), pos);
  }
  bool at(size_type pos) const {
    return at(root, pos);
  }
  size_type rank(size_type pos) const {
    return rank(root, pos);
  }
  size_type rank(size_type pos, bool bit) const {
    if(bit) {
      return rank(root, pos);
    }
    else {
      return pos - rank(root, pos);
    }
  }
  size_type size() const {
    return root->size();
  }

  void debug_tree(node_reference node, std::string d) const {
    if(node->is_leaf) {
      std::cout << d << "leaf " << node->size() << " " << node->popcount() << " -----------" << std::endl;
      std::cout << d << std::bitset<bit_limit * 2>(node->bits()) << std::endl;
      std::cout << d << "-------------" << std::endl;
    }
    else {
      std::cout << d << "node " << node->size() << " " << node->popcount() << " ----------" << std::endl;
      std::cout << d << "left" << std::endl;
      debug_tree(node->child(0), d + "   ");
      std::cout << d << "--------------" << std::endl;
      std::cout << d << "right" << std::endl;
      debug_tree(node->child(1), d + "   ");
      std::cout << d << "--------------" << std::endl;
    }
  }

  void debug_tree() const {
    debug_tree(root, "");
  }
};

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

  dynamic_wavelet_matrix(const std::vector<integer_type>& arr, size_type de)
    : depth(de),
      mat(de),
      len(arr.size()),
      spl(std::vector<size_type>(depth, 0)) {
        std::vector<size_type> idx(len);
        std::vector<size_type> left(len), right(len);

        for(size_type i = 0;i < len;i++) idx[i] = i;

        for(size_type d = depth; d-- > 0;) {
          size_type l = 0, r = 0;
          dynamic_bitvector::bitvector_builder builder(len);
          for(size_type i = 0; i < len; i++) {
            size_type k = (arr[idx[i]] >> d) & 1;
            if(k) right[r++] = idx[i], builder.set(i);
            else left[l++] = idx[i];
          }
          mat[d] = builder.build();
          spl[d] = l;
          swap(idx, left);
          for(size_type i = 0; i < r; i++) idx[i + l] = right[i];
        }
      }

  void insert(size_type i, integer_type x) {
    for(size_type d = depth; d --> 0;) {
      size_type k = ((x >> d) & 1);
      mat[d].insert(i, k);
      i = mat[d].rank(i, k) + spl[d] * k;
      if(k == 0) spl[d]++;
    }
  }

  void erase(size_type i) {
    for(size_type d = depth; d--> 0;) {
      size_type k = mat[d].at(i);
      mat[d].erase(i);
      i = mat[d].rank(i, k) + spl[d] * k;
      if(k == 0) spl[d]--;
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


int main() {
  cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  i64 N, Q;
  cin >> N >> Q;
  vector<i64> A(N);
  rep(i,0,N) cin >> A[i];
  
  dynamic_wavelet_matrix wm(A, 30);

  while(Q--) {
    i64 l, r, k;
    cin >> l >> r >> k;
    cout << wm.quantile(l, r, k) << "\n";
  }
}
