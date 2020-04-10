#include <cstdint>
#include <algorithm>
#include <iostream>
#include <cassert>

struct retroactive_stack {
  using value_type = long long;
  using size_type = std::size_t;
  using height_type = std::int_least32_t;
  using node_index = std::int_least32_t;

  struct diff {
    value_type p_value;
    height_type d;
    height_type max_d;
    height_type min_d;

    static diff push_diff(value_type value) {
      diff i;
      i.p_value = value;
      i.d = 1;
      i.max_d = 1;
      i.min_d = 0;
      return i;
    }
    static diff pop_diff() {
      diff i;
      i.d = -1;
      i.max_d = 0;
      i.min_d = -1;
      return i;
    }

    static diff ope(const diff& l, const diff& r) {
      diff i;
      i.d = l.d + r.d;
      i.max_d = std::max(l.max_d, l.d + r.max_d);
      i.min_d = std::min(l.min_d, l.d + r.min_d);
      return i;
    }
  };

  struct node;

  static struct node n[2020202];
  static size_type ni;

  struct node {
    diff d;
    size_type s;
    height_type h;
    node_index c[2];
    node_index p;
    node() {}
    node& operator[](size_type d) { return n[c[d]]; }
  };

  static node_index new_node(const diff& d) {
    node_index i = ni++;
    n[i].s = 1;
    n[i].h = 1;
    n[i].d = d;
    return i;
  }
  static void fix(node_index i) {
    n[i].s = n[i][0].s + n[i][1].s;
    n[i].h = std::max(n[i][0].h, n[i][1].h) + 1;
    n[i].d = diff::ope(n[i][0].d, n[i][1].d);
  }

  static size_type child_dir(node_index i) {
    if(n[i].p) {
      if(n[n[i].p].c[0] == i) { return 0; }
      else if(n[n[i].p].c[1] == i) { return 1; }
      assert(false);
    }
    return -1;
  }
  static node_index rotate(node_index x, size_type dir) {
    node_index p = n[x].p;
    size_type x_dir = child_dir(x);
    node_index y = n[x].c[1 ^ dir];
    n[n[y][dir].p = x].c[1 ^ dir] = n[y].c[dir];
    n[n[x].p = y].c[dir] = x;
    n[y].p = p;
    if(p > 0) n[p].c[x_dir] = y;
    fix(x);
    fix(y);
    return y;
  }

  static node_index balance(node_index i) {
    fix(i);
    if(n[i][0].h - n[i][1].h == 2) {
      if(n[i][0][0].h - n[i][0][1].h == -1) {
        rotate(n[i].c[0], 0);
      }
      return rotate(i, 1);
    }
    else if(n[i][0].h - n[i][1].h == -2) {
      if(n[i][1][0].h - n[i][1][1].h == 1) {
        rotate(n[i].c[1], 1);
      }
      return rotate(i, 0);
    }
    return i;
  }


  static node_index merge_dir(node_index l, node_index root, node_index r, size_type dir) {
    while(std::abs(n[l].h - n[r].h) > 1) {
      l = n[l].c[dir];
    }
    node_index x = n[l].p;
    n[n[l].p = root].c[dir ^ 1] = l;
    n[n[r].p = root].c[dir] = r;
    fix(root);
    n[n[root].p = x].c[dir] = root;
    x = root;
    while(n[x].p > 0) {
      x = n[x].p;
      x = balance(x);
    }
    return x;
  }

  static node_index merge(node_index l, node_index r) {
    if(!l) return r;
    else if(!r) return l;
    else if(n[l].h >= n[r].h) {
      return merge_dir(l, new_node(diff()), r, 1);
    }
    else {
      return merge_dir(r, new_node(diff()), l, 0);
    }
  }

  static std::pair<node_index, node_index> split(node_index root, size_type pos) {
    if(pos == 0) return { 0, root };
    if(pos == n[root].s) return { root, 0 };
    node_index i = root;
    node_index par = -1;
    while(i > 0 && pos != n[i][0].s) {
      if(pos < n[i][0].s) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].s;
        i = n[i].c[1];
      }
    }
    node_index l = n[i].c[0];
    n[l].p = 0;
    node_index r = n[i].c[1];
    n[r].p = 0;
    size_type dir;
    node_index p = n[i].p;
    node_index pd = child_dir(i);
    while(dir = pd, i = p, i > 0) {
      //std::cout << i << " " << dir << std::endl;
      pd = child_dir(i);
      p = n[i].p;
      n[i].p = 0;
      if(dir == 0) {
        n[i][1].p = 0;
        //std::cout << "merge_dir, 0" << std::endl;
        //debug(n[i].c[1], "");
        //debug(r, "");
        r = merge_dir(n[i].c[1], i, r, 0);
        //debug(r, "");
      }
      else {
        n[i][0].p = 0;
        //std::cout << "merge_dir, 0" << std::endl;
        //debug(n[i].c[0], "");
        //debug(l, "");
        l = merge_dir(n[i].c[0], i, l, 1);
        //debug(l, "");
      }
    }
    return { l, r };
  }

  static node_index at(node_index i, size_type pos) {
    while(n[i].c[0]) {
      if(pos < n[i][0].s) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].s;
        i = n[i].c[1];
      }
    }
    return i;
  }

  static void set(node_index i, size_type pos, diff v) {
    node_index par = -1;
    while(n[i].c[0]) {
      if(pos < n[i][0].s) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].s;
        i = n[i].c[1];
      }
    }
    n[i].d = v;

    while(i = n[i].p, i > 0) {
      fix(i);
    }
  }

  static size_type index(node_index i) {
    size_type pos = 0;
    while(n[i].p > 0) {
      if(child_dir(i) == 1) {
        pos += n[n[i].p][0].s;
      }
      i = n[i].p;
    }
    return pos;
  }

  static node_index search(node_index i, height_type pos) {
    while(n[i].c[0]) {
      if(n[i][0].d.d + n[i][1].d.min_d <= pos && pos < n[i][0].d.d + n[i][1].d.max_d) {
        pos -= n[i][0].d.d;
        i = n[i].c[1];
      }
      else {
        i = n[i].c[0];
      }
    }
    return i;
  }

  static std::pair<node_index, node_index> min_depth_split(node_index i) {
    node_index r = i;
    size_type pos = n[i].d.min_d;
    size_type res = 0;
    while(n[i].c[0]) {
      if(n[i][0].d.d + n[i][1].d.min_d <= pos && pos < n[i][0].d.d + n[i][1].d.max_d) {
        pos -= n[i][0].d.d;
        res += n[i][0].s;
        i = n[i].c[1];
      }
      else {
        i = n[i].c[0];
      }
    }
    return split(r, res);
  }

  static void debug(node_index i, std::string s) {
    if(i == 0) {
      std::cout << s << 0 << std::endl;
      return;
    }
    std::cout << s << i << " = " << n[i].d.p_value << " = " << n[i].p << " = " << n[i].s << std::endl;
    if(n[i].c[0] && n[i][0].p != i) {
      assert(false);
    }
    debug(n[i].c[0], s + "  ");
    if(n[i].c[1] && n[i][1].p != i) {
      assert(false);
    }
    debug(n[i].c[1], s + "  ");
  }


public:

  node_index root;
  retroactive_stack(): root(0) {}
  retroactive_stack(node_index i): root(i) {}

  static node_index new_push_operation(value_type val) {
    return new_node(diff::push_diff(val));
  }
  static node_index new_pop_operation() {
    return new_node(diff::pop_diff());
  }

  retroactive_stack& merge(retroactive_stack right) {
    root = merge(this->root, right.root);
    return *this;
  }
  retroactive_stack split(node_index i) {
    auto p = split(root, index(i));
    retroactive_stack avl;
    avl.root = p.second;
    root = p.first;
    return avl;
  }

  void update(node_index i, diff v) {
    set(root, index(i), v);
  }

  size_type operation_size() {
    return n[root].s;
  }

  height_type stack_size() {
    return n[root].d.d;
  }

  value_type top() {
    auto P = min_depth_split(root);
    auto res = n[search(P.second, n[P.second].d.d - 1)].d.p_value;
    root = merge(P.first, P.second);
    return res;
  }

  void debug() {
    debug(root, "");
  }
};

retroactive_stack::size_type retroactive_stack::ni = 1;
retroactive_stack::node retroactive_stack::n[2020202];

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
  std::vector<int> opes;
  opes.push_back(retroactive_stack::new_push_operation(1));
  opes.push_back(retroactive_stack::new_push_operation(2));
  opes.push_back(retroactive_stack::new_push_operation(3));

  retroactive_stack st;

  st.merge(retroactive_stack::new_pop_operation());
  st.merge(retroactive_stack::new_pop_operation());
  st.merge(retroactive_stack::new_pop_operation());

  for(auto i: opes) {
    st.merge(retroactive_stack(i));
  }
  std::cout << st.top() << std::endl; // 3

  int pop_ope = retroactive_stack::new_pop_operation();

  st.merge(retroactive_stack(pop_ope));

  std::cout << st.top() << std::endl; // 2

  st.merge(retroactive_stack(retroactive_stack::new_pop_operation()));
  std::cout << st.top() << std::endl; // 1

  st.update(pop_ope, retroactive_stack::diff::push_diff(4));
  std::cout << st.top() << std::endl; // 3 (push 1 -> push 2 -> push 3 -> push 4 -> pop)

  auto after = st.split(opes[2]);
  std::cout << st.top() << std::endl; // 2 st := (push 1 -> push 2)
}
