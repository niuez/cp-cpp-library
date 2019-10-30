#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

struct splay_map {
  using T = i64;
  using Key = i64;

  struct node {
    node* ch[2];
    node* par;
    T val;
    Key key;
    i64 sz;
    bool rev;

    /* option */
    //T fold;
    
    node(Key key, T v): key(key), val(v), par(nullptr), sz(1), rev(false)/*, fold(v)*/ { ch[0] = nullptr; ch[1] = nullptr; }
  };

private:

  i64 subsize(node* x) {
    if(x) return x->sz;
    else return 0;
  }
  /*
  T fold(node* x) {
    if(x) return x->fold;
    else return 0;
  }
  */
  node* fix(node* n) {
    if(!n) return nullptr;
    n->sz = subsize(n->ch[0]) + subsize(n->ch[1]) + 1;
    /* option */
    //n->fold = fold(n->ch[0]) + n->val + fold(n->ch[1]);
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

  node* root;

public:

  splay_map(): root(nullptr) {}

  node* lower_bound(Key key) {
    node* z = root;
    node* x = nullptr;
    while(z) {
      if(key <= z->key) {
        x = z;
        z = z->ch[0];
      }
      else {
        z = z->ch[1];
      }
    }
    return x;
  }

  void insert(Key key, T t) {
    node* z = new node(key, t);
    node* x = lower_bound(key);
    if(!x) {
      set(z, root, 0);
      root = z;
    }
    else {
      root = splay(x);
      node* q = root->ch[0];
      set(z, q, 0);
      set(root, z, 0);
    }
  }

  void erase(Key key) {
    node* x = lower_bound(key);
    if(x->key == key) {
      root = splay(x);
      node* p = root->ch[0];
      node* q = root->ch[1];
      root->ch[0] = nullptr;
      root->ch[1] = nullptr;
      if(p) p->par = nullptr;
      if(q) q->par = nullptr;
      if(!p) {
        root = q;
      }
      else {
        while(p->ch[1]) p = p->ch[1];
        root = splay(p);
        set(root, q, 1);
      }
    }
  }

  i64 size() {
    return subsize(root);
  }
  Key nth_node(i64 n) {
    node* z = root;
    while(z) {
      if(subsize(z->ch[0]) == n) {
        return z->key;
      }
      if(subsize(z->ch[0]) < n) {
        n -= subsize(z->ch[0]) + 1;
        z = z->ch[1];
      }
      else {
        z = z->ch[0];
      }
    }
    assert(false);
  }
  

  /* option */
  //T fold() { return fold(root); }
};
