#include <utility>
#include <array>
#include <cassert>

using i64 = long long;


#include <vector>
#include <iostream>
#include <string>
#include <cassert>
using i64 = long long;

namespace toptree {

  struct cluster {
    i64 sub;
    i64 sub_cnt;

    using V = std::pair<i64, i64>;
    cluster() {}
    cluster(i64 a, i64 b): sub(a), sub_cnt(b) {}
    static cluster identity() {
      return cluster(0, 0);
    }
    static cluster compress(const cluster& a, const cluster& b, V av, V bv, V cv) {
      return cluster(a.sub + b.sub + cv.first, a.sub_cnt + b.sub_cnt + cv.second);
    }
    static cluster rake(const cluster& a, const cluster& b, V av, V bv, V cv) {
      return cluster(a.sub + b.sub + bv.first, a.sub_cnt + b.sub_cnt + bv.second);
    }
    static V v_identity() {
      return { 0, 0 };
    }
    static cluster reverse(const cluster& c) {
      return c;
    }
    static std::size_t select(const cluster& a, const cluster& b, V av, V bv, V cv) {
      return 0;
    }
  };

  struct vertex;
  struct node;

  using size_type = std::size_t;
  using node_index = std::uint_least32_t;
  using vertex_index = std::uint_least32_t;

  extern struct vertex v[404040];
  extern size_type vi;
  extern struct node n[2020202];
  extern size_type ni;
  extern node_index guard;

  void link(node_index a, node_index b, cluster weight);

  struct vertex {
    cluster::V val;
    node_index hn;
  };

   vertex_index new_vertex(cluster::V val) {
    v[vi++] = { val, 0 };
    v[vi++] = { cluster::v_identity(), 0 };
    link(vi - 2, vi - 1, cluster::identity());
    return vi - 2;
  }


  enum class type { Compress, Rake, Edge };


  struct node {
    node_index i;
    node_index c[4];
    bool rev;
    cluster f;
    vertex_index v[2];
    type ty;
    i64 add;

    inline node& operator[](size_type d) { return n[c[d]]; }
    inline vertex& operator()(size_type d) { return toptree::v[this->v[d]]; }
  };

  static std::string type_string(type t) {
    if(t == type::Compress) return "C";
    if(t == type::Rake) return "R";
    return "E";
  }

  static void debug_tree(node_index i, std::string ind) {
    if(!i) return;
    std::cout << ind << type_string(n[i].ty) << " " << i << "(" << n[i].v[0] << " " << n[i].v[1] << ")" << " = " << n[i].f.sub_cnt << std::endl;
    debug_tree(n[i].c[0], ind + "| ");
    debug_tree(n[i].c[1], ind + "| ");
    debug_tree(n[i].c[2], ind + "| ");
  }

  inline node_index new_node(type ty) {
    node_index i = ni++;
    n[i].i = i;
    n[i].ty = ty;
    return i;
  }

   void reverse(node_index i) {
    std::swap(n[i].v[0], n[i].v[1]);
    n[i].f = cluster::reverse(n[i].f);
    n[i].rev ^= true;
  }

  inline void additive(node_index i, i64 x);

   void push(node_index i) {
    if(n[i].ty != type::Edge && n[i].rev) {
      std::swap(n[i].c[0], n[i].c[1]);
      reverse(n[i].c[0]);
      reverse(n[i].c[1]);
      n[i].rev = false;
    }
    if(n[i].ty == type::Compress) {
      additive(n[i].c[0], n[i].add);
      additive(n[i].c[1], n[i].add);
      if(n[i].c[2]) additive(n[i].c[2], n[i].add);
      n[i].add = 0;
    }
    else if(n[i].ty == type::Rake) {
      additive(n[i].c[0], n[i].add);
      additive(n[i].c[1], n[i].add);
      n[i].add = 0;
    }
  }

    void fix(node_index i) {
    push(i);
    if(n[i].ty == type::Compress) {
      n[i].v[0] = n[i][0].v[0];
      n[i].v[1] = n[i][1].v[1];
      cluster l = n[i][0].f;
      if(n[i].c[2])
        l = cluster::rake(l, n[i][2].f, n[i][0](0).val, n[i][2](0).val, n[i][0](1).val);
      n[i].f = cluster::compress(l, n[i][1].f, n[i][0](0).val, n[i][1](1).val, n[i][0](1).val);
    }
    if(n[i].ty == type::Rake) {
      n[i].v[0] = n[i][0].v[0];
      n[i].v[1] = n[i][0].v[1];
      n[i].f = cluster::rake(n[i][0].f, n[i][1].f, n[i][0](0).val, n[i][1](0).val, n[i][0](1).val);
    }

    if(n[i].ty == type::Compress)
      n[i][1](0).hn = i;
    if(n[i].ty != type::Rake) {
      if(!n[i].c[3])
        n[i](0).hn = n[i](1).hn = i;
      else if(n[i][3].ty == type::Rake || n[i][3].c[2] == n[i].i)
        n[i](0).hn = i;
    }
  }


   int child_dir(node_index i) {
    if(n[i].c[3]) {
      if(n[i][3].c[0] == i) { return 0; }
      else if(n[i][3].c[1] == i) { return 1; }
      else { return 2; }
    }
    return 3;
  }

  inline void additive(node_index i, i64 x) {
    n[i].f.sub += x * n[i].f.sub_cnt;

    auto func = [&](int dir) {
      n[i](dir).val = { n[i](dir).val.first + n[i](dir).val.second * x, n[i](dir).val.second };
    };
    auto func_center = [&]() {
      auto a = n[i][0].v[1];
      auto b = n[i][1].v[0];
      if(a == b)
        v[a].val = { v[a].val.first + v[a].val.second * x, v[a].val.second };
      else {
        a = n[i][0].v[0];
        v[a].val = { v[a].val.first + v[a].val.second * x, v[a].val.second };
      }
    };


    if(n[i].ty == type::Edge) {
      if(!n[i].c[3]) {
        func(0);
        func(1);
      }
      else if(n[i][3].ty == type::Compress) {
        if(child_dir(i) == 3) {
          func(0);
        }
      }
      else if(n[i][3].ty == type::Rake) {
        func(0);
      }
    }
    else if(n[i].ty == type::Compress) {
      n[i].add += x;
      func_center();
      if(!n[i].c[3]) {
        func(0);
        func(1);
      }
      else if(n[i][3].ty == type::Compress) {
        if(child_dir(i) == 3) {
          func(0);
        }
      }
      else if(n[i][3].ty == type::Rake) {
        func(0);
      }

    }
    else if(n[i].ty == type::Rake) {
      n[i].add += x;
    }
    else { assert(false); }
  }


   void rotate(node_index x, size_type dir) {
    node_index p = n[x].c[3];
    int x_dir = child_dir(x);
    node_index y = n[x].c[dir ^ 1];

    n[n[y][dir].c[3] = x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[3] = y].c[dir] = x;
    n[y].c[3] = p;
    if(x_dir <= 2) n[p].c[x_dir] = y;
    fix(n[x].c[dir ^ 1]);
    fix(x);
    if(n[x].c[3] && guard != n[x].c[3]) fix(n[x].c[3]);
  }

   void splay(node_index i) {
    push(i);
    int i_dir;
    int j_dir;
    while(child_dir(i) < 2 && n[i].c[3] != guard && n[i].ty == n[i][3].ty) {
      node_index j = n[i].c[3];
      if(child_dir(j) < 2 && n[j].c[3] != guard && n[j].ty == n[j][3].ty) {
        node_index k = n[j].c[3];
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

   void pusher(node_index t) {
     if(t && t != guard) {
       pusher(n[t].c[3]);
       push(t);
     }
   }

   node_index node_root(node_index i) {
     while(n[i].c[3]) i = n[i].c[3];
     return i;
   }

   node_index expose_raw(node_index i) {
    pusher(i);
    while(true) {
      if(n[i].ty == type::Compress) splay(i);
      node_index p = n[i].c[3];
      if(!p) break;
      else if(n[p].ty == type::Rake) {
        splay(p);
        p = n[p].c[3];
      }
      else if(p == toptree::guard && child_dir(i) < 2) break;

      splay(p);

      int dir = child_dir(p);
      dir = (dir >= 2 || n[p][3].ty == type::Rake) ? 0 : dir;
      if(dir == 1) {
        reverse(n[p].c[dir]);
        push(n[p].c[dir]);
        reverse(i);
        push(i);
      }

      int i_dir = child_dir(i);
      int x = n[i].c[3];
      int m = n[p].c[dir];

      n[n[m].c[3] = x].c[i_dir] = m;
      n[n[i].c[3] = p].c[dir] = i;
      fix(m);
      fix(x);
      fix(i); fix(p);
      if(n[x].ty == type::Rake) {
        splay(x);
      }
      if(n[i].ty == type::Edge) {
        i = p;
      }
    }
    return i;
  }

   node_index expose(vertex_index i) {
    return expose_raw(v[i].hn);
  }

   void soft_expose(vertex_index a, vertex_index b) {
    node_index r = expose(a);
    if(v[a].hn == v[b].hn) {
      if(n[r].c[1] == a || n[r].c[0] == b) reverse(r), push(r);
      return;
    }
    guard = r;
    node_index s = expose(b);
    guard = ~0;
    fix(r);
    if(child_dir(s) == 0) reverse(r), push(r);
  }

   void link(vertex_index a, vertex_index b, cluster weight) {
    node_index e = new_node(type::Edge);
    n[e].v[0] = a; n[e].v[1] = b; n[e].f = weight;
    if(!v[a].hn && !v[b].hn) { fix(e); return; }
    node_index br = node_root(v[b].hn);
    assert(node_root(v[b].hn) != node_root(v[a].hn));
    node_index na = v[a].hn;
    node_index nb = v[b].hn;
    node_index left;
    //debug_tree(br, "");
    for(int dir = 0; dir < 2; dir++) {
      if(!nb) left = e;
      else {
        nb = expose_raw(nb);
        if(n[nb].v[dir ^ 1] == b) reverse(nb);
        push(nb);

        if(n[nb].v[dir] == b) {
          left = new_node(type::Compress);
          n[left].c[dir] = e; n[left].c[dir ^ 1] = nb;
          n[e].c[3] = n[nb].c[3] = left;
          fix(e); fix(nb); fix(left);
        }
        else {
          node_index ch = n[nb].c[dir];
          if(dir) reverse(ch);
          push(ch);
          n[n[e].c[3] = nb].c[dir] = e;
          node_index beta = n[nb].c[2];
          push(beta);
          node_index rake;
          if(beta) {
            rake = new_node(type::Rake);
            n[rake].c[0] = beta; n[rake].c[1] = ch;
            n[beta].c[3] = n[ch].c[3] = rake;
            fix(beta); fix(ch);
          }
          else rake = ch;
          n[n[rake].c[3] = nb].c[2] = rake;
          fix(rake); fix(e); fix(left = nb);
        }
      }
      e = left;
      nb = na;
      b = a;
    }
  }

   cluster path_query(vertex_index a, vertex_index b) {
     soft_expose(a, b);
     node_index r = v[a].hn;
     if(n[r].v[0] == a && n[r].v[1] == b) return n[r].f;
     if(n[r].v[0] == a) return n[r][0].f;
     if(n[r].v[1] == b) return n[r][1].f;
     push(n[r].c[1]);
     return n[r][1][0].f;
   }

   void bring(node_index r, int dir) {
     push(r);
     node_index i = n[r].c[2];
     if(!i) {
       i = n[r].c[dir ^ 1];
       n[i].c[3] = 0;
       fix(i);
     }
     else if(n[i].ty == type::Rake) {
       while(push(i), n[i][1].ty == type::Rake) i = n[i].c[1];
       pusher(i);
       splay(i);
       n[n[i][0].c[3] = r].c[2] = n[i].c[0];
       if(dir) reverse(n[i].c[1]);
       push(n[i].c[1]);
       n[n[i][1].c[3] = r].c[dir] = n[i].c[1];
       fix(n[r].c[2]); fix(n[r].c[dir]); fix(r);
     }
     else {
       if(dir) reverse(i);
       push(i);
       n[n[i].c[3] = r].c[dir] = i;
       n[r].c[2] = 0;
       fix(n[r].c[dir]); fix(r);
     }
   }

   void cut(vertex_index a, vertex_index b) {
     soft_expose(a, b);
     node_index r = v[a].hn;
     node_index s = v[b].hn;
     push(r);
     push(s);
     n[s].c[3] = 0;
     n[r].c[1] = 0;
     bring(r, 1);
     bring(s, 0);
   }

   i64 all_tree(vertex_index a) {
     expose(a);
     return n[v[a].hn].f.sub + n[v[a].hn](0).val.first + n[v[a].hn](1).val.first;
   }

}
