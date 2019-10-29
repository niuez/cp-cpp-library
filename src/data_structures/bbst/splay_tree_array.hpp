#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct splay_array {
  using T = i64;

  struct node {
    node* ch[2];
    node* par;
    T val;
    i64 sz;
    bool rev;

    /* option */
    T fold;
    
    node(T v): val(v), par(nullptr), sz(1), rev(false), fold(v) { ch[0] = nullptr; ch[1] = nullptr; }
  };

private:

  i64 subsize(node* x) {
    if(x) return x->sz;
    else return 0;
  }
  T fold(node* x) {
    if(x) return x->fold;
    else return 0;
  }
  node* fix(node* n) {
    if(!n) return nullptr;
    n->sz = subsize(n->ch[0]) + subsize(n->ch[1]) + 1;
    /* option */
    n->fold = fold(n->ch[0]) + n->val + fold(n->ch[1]);
    return n;
  }
  void reverse(node* n) {
    if(!n) return;
    n->rev ^= true;
    /* here reversing operation */
    /* example swap(fold, revfold) */
  }
  void push(node* x) {
    if(!x) return;
    if(x->rev) {
      swap(x->ch[0], x->ch[1]);
      reverse(x->ch[0]);
      reverse(x->ch[1]);
    }
  }
  int parent_dir(node* x) {
    node* p = x->par;
    if(!p) return -1;
    else if(p->ch[0] == x) return 0;
    else return 1;
  }
  void set(node* par, node* x, i64 dir) {
    if(par) par->ch[dir] = x;
    if(x) x->par = par;
    fix(par);
  }
  void rotate(node* x, i64 dir) {
    node* p = x->par;
    node* q = p->par;
    set(p, x->ch[dir], dir ^ 1);
    int p_dir = parent_dir(p);
    if(p_dir >= 0) {
      set(q, x, p_dir);
    }
    else x->par = nullptr;
    set(x, p, dir);
  }
  node* splay(node * x) {
    if(!x) return nullptr;
    while(x->par) {
      push(x->par->par);
      push(x->par);
      push(x);
      int dir = parent_dir(x);
      int eir = parent_dir(x->par);
      if(eir == -1) {
        rotate(x, dir  ^ 1);
      }
      else if(dir == eir){
        rotate(x->par, eir ^ 1);
        rotate(x, dir ^ 1);
      }
      else {
        rotate(x, dir ^ 1);
        rotate(x, eir ^ 1);
      }
    }
    return x;
  }

  node* find(node* r, i64 i) {
    push(r);
    assert(0 <= i);
    assert(i < subsize(r));
    node* z = r;
    while(z) {
      push(z);
      if(subsize(z->ch[0]) == i) return splay(z);
      else if(subsize(z->ch[0]) < i) {
        i -= subsize(z->ch[0]) + 1;
        z = z->ch[1];
      }
      else {
        z = z->ch[0];
      }
    }
    assert(false);
  }

  pair<node*, node*> split(node* r, size_t i) {
    push(r);
    assert(0 <= i);
    assert(i <= subsize(r));
    if(i == 0) return { nullptr, r };
    if(i == subsize(r)) return { r, nullptr };
    r = find(r, i - 1);
    node* y = r->ch[1];
    if(y) y->par = nullptr;
    r->ch[1] = nullptr;
    fix(r);
    push(y);
    return { r, y };
  }

  node* merge(node* r1, node* r2) {
    push(r1);
    push(r2);
    if(!r1) r1 = r2;
    else if(!r2) {}
    else {
      r1 = find(r1, subsize(r1) - 1);
      set(r1, r2, 1);
    }
    return r1;
  }

  node* root;

  splay_array(node* r): root(r) {}

public:

  using sarr = splay_array;

  splay_array(): root(nullptr) {}
  splay_array(T t): root(new node(t)) {}
  splay_array(splay_array&& arr): root(arr.root) { arr.root = nullptr; }
  splay_array& operator=(splay_array&& arr) {
    root = arr.root;
    arr.root = nullptr;
    return *this;
  }
  /* [0 ... i - 1] +/+ [i ...] */
  pair<splay_array, splay_array> split(i64 i) {
    auto p = split(root, i);
    root = nullptr;
    return { splay_array(p.first), splay_array(p.second) };
  }
  /* [this] ++ [arr] */
  void merge(splay_array&& arr) {
    root = merge(root, arr.root);
    arr.root = nullptr;
  }
  /* reverse array */
  void reverse() { if(root) reverse(root); }
  i64 size() { return subsize(root); }

  /* option */
  T fold() { return fold(root); }
  void update(i64 i, T t) {
    root = find(root, i);
    root->val += t;
    fix(root);
  }
};
