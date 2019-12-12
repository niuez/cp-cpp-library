#include <vector>
#include <set>
using namespace std;

struct union_find {
  vector<int> par;
  vector<int> rank;
  union_find(int n) : par(n) , rank(n) {
    for(int i = 0;i < n;i++) par[i] = i;
  }
  int root(int i) {
    return par[i] == i ? i : par[i] = root(par[i]);
  }
  /* unite x, y return parent */
  int unite(int x,int y) {
    x = root(x);
    y = root(y);
    if(x == y) return -1;
    if(rank[x] < rank[y]) {
      par[x] = y;
      return y;
    }
    else {
      par[y] = x;
      if(rank[x] == rank[y]) rank[x]++;
      return x;
    }
  }
};
