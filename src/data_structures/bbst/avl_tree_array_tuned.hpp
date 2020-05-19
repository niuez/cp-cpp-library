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

