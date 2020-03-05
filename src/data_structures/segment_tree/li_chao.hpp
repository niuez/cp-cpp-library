#include <functional>
using namespace std;

// doubleのときは, midを変える
template<class T,const T ide,class Compare = greater<T>>
struct li_chao{
  struct Line{
    T a,b;
    Line(T a = 0,T b = 0) : a(a) , b(b) {}
    T get(T x){return a * x + b;}
  };
 
  struct Node{
    Line line;
    Node *lhs,*rhs;
    Node(Line l) : line(l) , lhs(nullptr) , rhs(nullptr){}
  };
 
  const T MI,MA;
 
  Node * root;
 
  Compare comp;
 
  T comp_get(const T & x,const T & y){
    if(comp(x , y)) return x;
    else return y;
  }
 
  li_chao(T mi , T ma) : MI(mi), MA(ma) , root(nullptr){}
 
  Node * insert(Node * p,T l,T r,Line & line){
    if(l > r) {
      return p;
    }
    if(!p) return new Node(line);
    if(comp(p->line.get(l) , line.get(l)) && comp(p->line.get(r) ,line.get(r))){
      return p;
    }
    if(!comp(p->line.get(l) , line.get(l)) && !comp(p->line.get(r) ,line.get(r))){
      p->line = line;
      return p;
    }
    T mid = (l + r) / 2;
    if(comp(line.get(mid) , p->line.get(mid))) swap(p->line , line);
    if(comp(line.get(l) , p->line.get(l))){
      p->lhs = insert(p->lhs , l , mid , line);
    }
    else{
      p->rhs = insert(p->rhs , mid + 1, r , line);
    }
    return p;
  }
 
  void add_line(T a,T b){
    Line l(a , b);
    root = insert(root,MI,MA,l);
  }
 
  T get(Node * p,T l,T r,T t){
    if(!p) return ide;
    T mid = (l + r) / 2;
    if(t <= mid) return comp_get(p->line.get(t) , get(p->lhs , l, mid,t));
    else return comp_get(p->line.get(t),get(p->rhs,mid + 1 ,r , t));
  }
 
  T get(T x){
    return get(root,MI,MA,x);
  }
};
