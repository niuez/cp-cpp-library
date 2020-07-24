#include <cstdint>
#include <set>
#include <string>
#include <iostream>

struct euler_tour_tree {
  using size_type = std::size_t;
  using node_index = std::int_least32_t;
  using vertex_index = std::int_least32_t;

  struct node;
  static struct node n[2020];
  static node_index ni;

  struct node {
    vertex_index s, d;
    node_index c[3];
    int sz;
    node(): sz(1) {}
    inline node& operator[](size_type d) { return n[c[d]]; }
  };

  node_index new_edge(int s, int d) {
    int i = ni++;
    int ri = ni++;
    n[i].s = n[ri].d = s;
    n[i].d = n[ri].s = d;
    return i;
  }

  static void fix(node_index i) {
    n[i].sz = (n[i].s == n[i].d) ? 1 : 0;
    if(n[i].c[0]) n[i].sz += n[i][0].sz;
    if(n[i].c[1]) n[i].sz += n[i][1].sz;
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
  euler_tour_tree(int N): n_start(ni) {
    ni += N;
    for(int i = 0; i < N; i++) {
      n[i + n_start].s = n[i + n_start].d = i;
    }
  }


  node_index link(vertex_index x, vertex_index y) {
    int ei = new_edge(x, y);
    x += n_start;
    y += n_start;
    reroot(x);
    reroot(y);
    n[n[x].c[2] = ei].c[0] = x;
    n[n[y].c[2] = ei].c[1] = y;
    fix(ei);
    merge_back(ei, ei + 1);
    return ei;
  }

  void cut(node_index ei) {
    int rei = ei + 1;
    auto p = split(ei);
    if(same_root(p.first, rei)) {
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
euler_tour_tree::node euler_tour_tree::n[2020];


int main() {
  euler_tour_tree ett(10);

  ett.link(0, 1);
  ett.link(1, 2);
  int ei = ett.link(1, 3);
  ett.link(0, 4);
  std::cout << "cut" << std::endl;
  euler_tour_tree::reroot(4);
  ett.cut(ei);
  euler_tour_tree::reroot(1);
  std::cout << "result" << std::endl;
  ett.debug_tree(1, "");
  std::cout << "result3" << std::endl;
  euler_tour_tree::reroot(4);
  ett.debug_tree(4, "");
}
