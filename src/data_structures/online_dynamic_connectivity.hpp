#include <cstdint>
#include <set>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

const int NODE_SIZE = 303030 * 6;
struct euler_tour_tree {
  using value_type = long long;
  using size_type = std::size_t;
  using node_index = std::int_least32_t;
  using vertex_index = std::int_least32_t;

  struct node;
  static struct node n[NODE_SIZE];
  static node_index ni;

  struct node {
    vertex_index s, d;
    node_index c[3];
    int sz;
    int flag;
    value_type val;
    value_type Sigma;
    node(): sz(1) {}
    inline node& operator[](size_type d) { return n[c[d]]; }
  };

  node_index new_edge(int s, int d, bool hi) {
    int i = ni++;
    int ri = ni++;
    n[i].s = n[ri].d = s;
    n[i].d = n[ri].s = d;
    n[i].sz = n[ri].sz = 0;
    n[i].flag = hi;
    return i;
  }

  static void fix(node_index i) {
    n[i].sz = (n[i].s == n[i].d) ? 1 : 0;
    if(n[i].c[0]) n[i].sz += n[i][0].sz;
    if(n[i].c[1]) n[i].sz += n[i][1].sz;
    n[i].flag &= 0b0101;
    n[i].flag |= n[i].flag << 1;
    if(n[i].c[0]) n[i].flag |= n[i][0].flag & 0b1010;
    if(n[i].c[1]) n[i].flag |= n[i][1].flag & 0b1010;
    n[i].Sigma = n[i].val;
    if(n[i].c[0]) n[i].Sigma += n[i][0].Sigma;
    if(n[i].c[1]) n[i].Sigma += n[i][1].Sigma;
  }

  static int child_dir(node_index i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) { return 0; }
      else if(n[i][2].c[1] == i) { return 1; }
    }
    return 2;
  }

  static void rotate(node_index x, size_type dir) {
    node_index p = n[x].c[2];
    int x_dir = child_dir(x);
    node_index y = n[x].c[dir ^ 1];
    if(n[y].c[dir]) n[y][dir].c[2] = x;
    n[x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(x_dir < 2) n[p].c[x_dir] = y;
    if(n[x].c[dir ^ 1]) fix(n[x].c[dir ^ 1]);
    fix(x);
  }

  static void splay(node_index i) {
    int i_dir;
    int j_dir;
    while((i_dir = child_dir(i)) < 2) {
      node_index j = n[i].c[2];
      if((j_dir = child_dir(j)) < 2) {
        node_index k = n[j].c[2];
        if(i_dir == j_dir) rotate(k, j_dir ^ 1), rotate(j, i_dir ^ 1);
        else rotate(j, i_dir ^ 1), rotate(k, j_dir ^ 1);
      }
      else rotate(j, i_dir ^ 1);
    }
    fix(i);
  }

  static node_index merge_back(node_index l, node_index r) {
    if(!l) return r;
    if(!r) return l;
    while(n[l].c[1]) l = n[l].c[1];
    splay(l);
    n[n[r].c[2] = l].c[1] = r;
    fix(l);
    return l;
  }

  static std::pair<node_index, node_index> split(node_index i) {
    splay(i);
    node_index l = n[i].c[0];
    n[i].c[0] = n[l].c[2] = 0;
    fix(i);
    return { l, i };
  }

  static void reroot(node_index v) {
    auto p = split(v);
    merge_back(p.second, p.first);
    splay(v);
  }

  static bool same_root(node_index i, node_index j) {
    if(i) splay(i);
    if(j) splay(j);
    while(n[i].c[2]) i = n[i].c[2];
    while(n[j].c[2]) j = n[j].c[2];
    return i == j;
  }

  node_index n_start;
  std::unordered_map<long long, node_index> emp;
  euler_tour_tree() {}
  euler_tour_tree(int N): n_start(ni) {
    ni += N;
    for(int i = 0; i < N; i++) {
      n[i + n_start].s = n[i + n_start].d = i;
    }
  }

  bool edge_exist(vertex_index x, vertex_index y) {
    if(x > y) std::swap(x, y);
    return emp.count(((long long)x << 32) | (long long)y);
  }

  void link(vertex_index x, vertex_index y, bool hi) {
    if(x > y) std::swap(x, y);
    int ei = new_edge(x, y, hi);
    assert(!emp.count(((long long)x << 32) | (long long)y));
    emp[((long long)x << 32) | (long long)y] = ei;
    x += n_start;
    y += n_start;
    reroot(x);
    reroot(y);
    n[n[x].c[2] = ei].c[0] = x;
    n[n[y].c[2] = ei].c[1] = y;
    fix(ei);
    merge_back(ei, ei + 1);
  }

  void cut(vertex_index x, vertex_index y) {
    if(x > y) std::swap(x, y);
    auto iter = emp.find(((long long)x << 32) | (long long)y);
    int ei = iter->second;
    int rei = ei + 1;
    emp.erase(iter);

    auto p = split(ei);
    if(p.first && same_root(p.first, rei)) {
      auto q = split(rei);
      node_index left = q.first;
      node_index center = n[q.second].c[1];
      node_index right = n[p.second].c[1];
      n[center].c[2] = 0;
      n[right].c[2] = 0;
      merge_back(left, right);
    }
    else {
      splay(ei);
      ei = n[ei].c[1];
      n[ei].c[2] = 0;
      auto q = split(rei);
      splay(p.first);
      node_index left = p.first;
      node_index center = q.first;
      node_index right = n[q.second].c[1];
      n[right].c[2] = 0;
      merge_back(left, right);
    }
  }

  bool same_tree(vertex_index x, vertex_index y) {
    return same_root(x + n_start, y + n_start);
  }

  int tree_size(vertex_index x) {
    x += n_start;
    splay(x);
    return n[x].sz;
  }

  void subedge_set(vertex_index x, bool val) {
    x += n_start;
    splay(x);
    if(val) n[x].flag |= (0b0100);
    else n[x].flag &= ~(0b0100);
    fix(x);
  }

  void add_val(vertex_index x, value_type val) {
    x += n_start;
    splay(x);
    n[x].val += val;
    fix(x);
  }
  value_type tree_sum(vertex_index x) {
    x += n_start;
    splay(x);
    return n[x].Sigma;
  }

  template<class Func>
  void hilevel_edges(vertex_index v, Func f) {
    node_index i = v + n_start;
    splay(i);
    while(i && (n[i].flag & 0b0010)) {
      while(1) {
        if(n[i].flag & 0b0001) {
          f(n[i].s, n[i].d);
          splay(i);
          n[i].flag &= ~(0b0001);
          fix(i);
          break;
        }
        else if(n[i].c[0] && (n[i][0].flag & 0b0010)) i = n[i].c[0];
        else i = n[i].c[1];
      }
    }
  }
  template<class Func>
  int subedges(vertex_index v, Func f) {
    node_index i = v + n_start;
    splay(i);
    while(i && (n[i].flag & 0b1000)) {
      while(1) {
        if(n[i].flag & 0b0100) {
          if(f(n[i].s)) {
            return 1;
          }
          splay(i);
          break;
        }
        else if(n[i].c[0] && (n[i][0].flag & 0b1000)) i = n[i].c[0];
        else i = n[i].c[1];
      }
    }
    return 0;
  }


  void debug_tree(node_index i, std::string indent) {
    if(n[i].c[0]) {
      debug_tree(n[i].c[0], indent + "l");
    }
    std::cout << " " << i << " = (" << n[i].s << " " << n[i].d << ")" << " p " << n[i].c[2] << std::endl;
    if(n[i].c[1]) {
      debug_tree(n[i].c[1], indent + "r");
    }
  }
};

euler_tour_tree::node_index euler_tour_tree::ni = 1;
euler_tour_tree::node euler_tour_tree::n[NODE_SIZE];

struct online_dynamic_connectivity {
  int N;
  std::vector<euler_tour_tree> ett;
  std::vector<std::vector<std::unordered_set<int>>> E;

  online_dynamic_connectivity(int N): N(N) {
    ett.emplace_back(N);
    E.emplace_back(N);
  }

  void link(int x, int y) {
    if(ett[0].same_tree(x, y)) {
      if(E[0][x].size() == 0) ett[0].subedge_set(x, 1);
      if(E[0][y].size() == 0) ett[0].subedge_set(y, 1);
      E[0][x].insert(y);
      E[0][y].insert(x);
    }
    else {
      ett[0].link(x, y, true);
    }
  }

  void replace(int x, int y, int level) {
    for(int k = 0; k < level; k++) {
      ett[k].cut(x, y);
    }
    for(int k = level; k --> 0;) {
      if(ett[k].tree_size(x) > ett[k].tree_size(y)) std::swap(x, y);
      ett[k].hilevel_edges(x, [&](int s, int d) { ett[k + 1].link(s, d, true); });
      int res = ett[k].subedges(x, [&](int s) {
        for(auto iter = E[k][s].begin(); iter != E[k][s].end(); ) {
          int d = *iter;
          iter = E[k][s].erase(iter);
          E[k][d].erase(s);
          if(E[k][s].size() == 0) ett[k].subedge_set(s, 0);
          if(E[k][d].size() == 0) ett[k].subedge_set(d, 0);
          if(ett[k].same_tree(s, d)) {
            if(E[k + 1][s].size() == 0) ett[k + 1].subedge_set(s, 1);
            if(E[k + 1][d].size() == 0) ett[k + 1].subedge_set(d, 1);
            E[k + 1][s].insert(d);
            E[k + 1][d].insert(s);
          }
          else {
            for(int kk = k + 1; kk --> 0;) {
              ett[kk].link(s, d, kk == k);
            }
            return 1;
          }
        }
        return 0;
        });
      if(res) return;
    }
  }

  void cut(int x, int y) {
    for(int k = 0; k < ett.size(); k++) {
      if(E[k][x].count(y)) {
        E[k][x].erase(y);
        E[k][y].erase(x);
        if(E[k][x].size() == 0) ett[k].subedge_set(x, 0);
        if(E[k][y].size() == 0) ett[k].subedge_set(y, 0);
        return;
      }
    }
    for(int k = ett.size(); k --> 0;) {
      if(ett[k].edge_exist(x, y)) {
        if(k + 1 == ett.size()) {
          ett.emplace_back(N);
          E.emplace_back(N);
        }
        replace(x, y, k + 1);
      }
    }
  }
  void add_val(int x, long long val) {
    ett[0].add_val(x, val);
  }
  int size(int x) {
    return ett[0].tree_size(x);
  }
  long long sum(int x) {
    return ett[0].tree_sum(x);
  }
  bool same(int x, int y) {
    return ett[0].same_tree(x, y);
  }
};

