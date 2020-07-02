#include <cstdint>
#include <set>
#include <cassert>
#include <iostream>
#include <string>

const int SPLAY_ARRAY_NODE = 1010101;

template<class Key, class Value>
struct splay_array {
  using key_type = Key;
  using value_type = Value;
  using node_index = std::int_least32_t;
  using size_type = std::int_least32_t;

  struct node;

  static struct node n[SPLAY_ARRAY_NODE];
  static node_index ni;

  struct node {
    node_index c[3];
    key_type key;
    value_type val;
    value_type f;
    size_type sz;
    bool rev;

    node() {}
    node& operator[](int dir) { return n[c[dir]]; }
  };

  static node_index new_node(const value_type& key, const value_type& val) {
    node_index i = ni++;
    n[i].key = key;
    n[i].val = val;
    n[i].f = val;
    n[i].sz = 1;
    return i;
  }

  static void fix(node_index i) {
    if(!i) return;
    n[i].sz = 1 + n[i][0].sz + n[i][1].sz;
    n[i].f = n[i][0].f + n[i].val + n[i][1].f;
  }
  static void reverse(node_index i) {
    if(!i) return;
    n[i].rev ^= true;
    /* here reversing operation */
    /* example swap(fold, revfold) */
  }
  static void push(node_index i) {
    if(!i) return;
    if(n[i].rev) {
      std::swap(n[i].c[0], n[i].c[1]);
      reverse(n[i].c[0]);
      reverse(n[i].c[1]);
      n[i].rev = false;
    }
  }
  static int child_dir(node_index i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) return 0;
      return 1;
    }
    return 2;
  }

  static void rotate(node_index x, size_type dir) {
    node_index p = n[x].c[2];
    int x_dir = child_dir(x);
    node_index y = n[x].c[dir ^ 1];

    n[n[y][dir].c[2] = x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(p) n[p].c[x_dir] = y;
    fix(n[x].c[dir ^ 1]);
    fix(x);
  }

  static void splay(node_index i) {
    push(i);
    int i_dir;
    int j_dir;
    while(n[i].c[2]) {
      node_index j = n[i].c[2];
      if(n[j].c[2]) {
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

  static node_index leftist_node(node_index i) {
    if(push(i), n[i].c[0]) i = n[i].c[0];
    return i;
  }

  static node_index merge_raw(node_index l, node_index r) {
    node_index leftist = leftist_node(r);
    splay(leftist);
    n[n[l].c[2] = leftist].c[0] = l;
    fix(leftist);
    return leftist;
  }

  node_index endn;
  node_index root;

  splay_array() {}
  splay_array(node_index i) {
    endn = ni++;
    root = endn;
    n[n[i].c[2] = root].c[0] = i;
    fix(root);
  }
  node_index end_node() const { return endn; }

  node_index find_by_index(size_type pos) {
    node_index i = root;
    while(push(i), n[i][0].sz != pos) {
      if(pos < n[i][0].sz) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].sz + 1;
        i = n[i].c[1];
      }
    }
    splay(root = i);
    return i;
  }

  node_index lower_bound_by_key(const key_type& key) {
    node_index i = root;
    node_index p = endn;
    while(push(i), i != 0) {
      if(i == endn) {
        i = n[i].c[0];
      }
      else if(key == n[i].key) {
        p = i;
        break;
      }
      else if(key < n[i].key) {
        p = i;
        i = n[i].c[0];
      }
      else {
        i = n[i].c[1];
      }
    }
    splay(root = p);
    return p;
  }

  void splay_node(node_index i) {
    splay(root = i);
  }

  void merge(splay_array right) {
    splay(endn);
    root = n[endn].c[0];
    root = merge_raw(n[endn].c[0], right.root);
    endn = right.endn;
  }
  
  /* i は右 戻り値は左 */
  splay_array split(node_index i) {
    splay(root = i);
    i = n[root].c[0];
    n[i].c[2] = n[root].c[0] = 0;
    fix(root);
    return splay_array(i);
  }

  void insert_dir(node_index i, node_index target, int dir) {
    splay(root = i);
    node_index x = n[i].c[dir];
    n[n[target].c[2] = i].c[dir] = target;
    n[n[x].c[2] = target].c[dir] = x;
    fix(target);
    fix(i);
  }

  void erase(node_index i) {
    assert(endn != i);
    splay(root = i);
    node_index l = n[i].c[0];
    node_index r = n[i].c[1];
    n[l].c[2] = n[r].c[2] = n[i].c[0] = 0;
    root = merge_raw(l, r);
  }

  static void debug_tree(node_index i, std::string ind, int cnt) {
    if(cnt >= 7) assert(false);
    if(i == 0) {
      std::cout << ind << " " << 0 << std::endl;
      return;
    }
    std::cout << ind << " " << i << " = " << n[i].key << " -> " << n[i].val << " - " << n[i].c[2] << " " << n[i].f << std::endl;
    debug_tree(n[i].c[0], ind + "  ", cnt + 1);
    debug_tree(n[i].c[1], ind + "  ", cnt + 1);
  }

  void debug_print() {
    debug_tree(root, "", 0);
  }
};

template<class K, class V>
typename splay_array<K, V>::node splay_array<K, V>::n[SPLAY_ARRAY_NODE];
template<class K, class V>
typename splay_array<K, V>::node_index splay_array<K, V>::ni = 1;
