#include <memory>
#include <array>
#include <set>
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
    bool dummy_;
    section_t section;
    leaf_t leaf;
    info_t(bool a): dummy_(a) {}
    ~info_t() {}
  };

  struct node {
    const bool is_leaf;
    size_type sz;
    size_type popcnt;
    info_t info;

    static node_type new_section(node_type left, node_type right) {
      node* n = new node(false);
      n->info.section = section_t { 0, children_type { std::move(left), std::move(right) }  };
      n->fix();
      return node_type(n);
    }

    static node_type new_leaf(bits_type bits, size_type sz, size_type popcnt) {
      node* n = new node(true);
      n->sz = sz;
      n->info.leaf = leaf_t { bits };
      n->popcnt = popcnt;
      return node_type(n);
    }

    node(bool is_leaf_): is_leaf(is_leaf_), info(false) {}

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

  static size_type rank(node_reference node, size_type pos) {
    if(node->is_leaf) {
      return node->rank(pos);
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
  void insert(size_type pos, bool bit) {
    root = insert(std::move(root), pos, bit);
  }
  size_type rank(size_type pos) const {
    return rank(root, pos);
  }
  size_type size() const {
    return root->size();
  }

  void debug_tree(node_reference node) const {
    if(node->is_leaf) {
      std::cout << "leaf " << node->size() << " -----------" << std::endl;
      std::cout << std::bitset<bit_limit * 2>(node->bits()) << std::endl;
      std::cout << "-------------" << std::endl;
    }
    else {
      std::cout << "node " << node->size() << " ----------" << std::endl;
      std::cout << "left" << std::endl;
      debug_tree(node->child(0));
      std::cout << "right" << std::endl;
      debug_tree(node->child(1));
      std::cout << "--------------" << std::endl;
    }
  }

  void debug_tree() const {
    debug_tree(root);
  }
};

#include <vector>

int main() {
  std::vector<int> bits;
  dynamic_bitvector bitvec;
  const int len = 128;
  for(int i = 0;i < len;i++) {
    int pos = rand() % (i + 1);
    bool bit = rand() % 2;
    bits.insert(std::begin(bits) + pos, bit);
    bitvec.insert(pos, bit);
  }

  std::bitset<len> bs;
  for(int i = 0;i < len;i++) bs[i] = bits[i];
  std::cout << bs << std::endl;
  bitvec.debug_tree();
  int sum = 0;
  for(int i = 0;i < bits.size();i++) {
    std::cout << i << " " << bitvec.rank(i) << " : " << sum << std::endl;
    assert(bitvec.rank(i) == sum);
    sum += bits[i];
  }
}
