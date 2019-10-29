#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

namespace link_cut_tree {
  struct node {
    node* ch[2];
    node* par;
    bool rev;

    i64 sz;
    i64 val;
    i64 fold;

    i64 lazy;
    node(i64 v) {
      ch[0] = nullptr;
      ch[1] = nullptr;
      par = nullptr;
      rev = false;

      sz = 1;
      val = v;
      fold = v;

      lazy = 0;
    }
  };

  i64 size(node* n) {
    if(n) return n->sz;
    return 0;
  }

  i64 fold(node* n) {
    if(n) return n->fold;
    return 0;
  }
  

  void fix(node * n) {
    if(n) {
      /* fix operation here */
      n->sz = 1 + size(n->ch[0]) + size(n->ch[1]);
      n->fold = n->val + fold(n->ch[0]) + fold(n->ch[1]);
    }
  }

  void reverse(node* n) {
    if(n) {
      n->rev ^= true;
      swap(n->ch[0], n->ch[1]);
      /* reverse operation here */
    }
  }

  void lazy(node* n, i64 l) {
    if(n) {
      n->lazy += l;
      n->val += l;
      n->fold += l * size(n);
    }
  }

  void push(node* n) {
    if(n) {
      if(n->rev) {
        reverse(n->ch[0]);
        reverse(n->ch[1]);
      }
      lazy(n->ch[0], n->lazy);
      lazy(n->ch[1], n->lazy);
      n->lazy = 0;
    }
  }

  i64 pdir(node* n) {
    if(n->par) {
      if(n->par->ch[0] == n) return 0;
      if(n->par->ch[1] == n) return 1;
    }
    return -1;
  }

  void rot(node* n, i64 dir) {
    node* x = n->par;
    node* y = n->par->par;
    i64 pd = pdir(x);
    x->ch[dir ^ 1] = n->ch[dir];
    if(n->ch[dir]) n->ch[dir]->par = x;
    n->ch[dir] = x;
    x->par = n;
    fix(x);
    fix(n);
    n->par = y;
    if(pd >= 0) {
      y->ch[pd] = n;
      fix(y);
    }
  }

  void splay(node* t) {
    while(pdir(t) >= 0) {
      node* q = t->par;
      if(pdir(q) == -1) {
        push(q);
        push(t);
        rot(t, pdir(t) ^ 1);
      }
      else {
        node* r = q->par;
        push(r);
        push(q);
        push(t);
        i64 rq = pdir(q) ^ 1;
        i64 qt = pdir(t) ^ 1;
        if(rq == qt) {
          rot(q, rq);
          rot(t, qt);
        }
        else {
          rot(t, qt);
          rot(t, rq);
        }
      }
    }
  }

  /* 一番上のsplay treeが[root, t]を表すようになる */
  void expose(node* t) {
    node* rp = nullptr;
    for(node* cur = t; cur; cur = cur->par) {
      splay(cur);
      cur->ch[1] = rp;
      fix(cur);
      rp = cur;
    }
    splay(t);
  }

  void link(node* p, node* c) {
    expose(c);
    expose(p);
    p->ch[1] = c;
    c->par = p;
  }
  
  /* cut edge (c->par, c) */
  void cut(node* c) {
    expose(c);
    node* p = c->ch[0];
    c->ch[0] = nullptr;
    p->par = nullptr;
  }

  void evert(node* t) {
    expose(t);
    reverse(t);
    push(t);
  }
};

int main() {
  i64 n;
  cin >> n;
  
  namespace lct = link_cut_tree;

  vector<lct::node*> ns;
  for(int i = 0;i < n;i++) {
    ns.push_back(new lct::node(0ll));
  }

  for(int i = 0;i < n;i++) {
    i64 k;
    cin >> k;
    for(int j = 0;j < k;j++) {
      i64 c;
      cin >> c;
      lct::link(ns[i], ns[c]);
    }
  }

  i64 Q;
  cin >> Q;
  for(int q = 0; q < Q; q++) {
    i64 type = 0;
    cin >> type;
    if(type == 0) {
      i64 v, w;
      cin >> v >> w;
      lct::expose(ns[v]);
      lct::lazy(ns[v], w);
      push(ns[v]);
    }
    else {
      i64 u;
      cin >> u;
      lct::expose(ns[u]);
      i64 val = ns[u]->fold;
      lct::expose(ns[0]);
      val -= ns[0]->val;
      cout << val << endl;
    }
  }
}
