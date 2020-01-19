#include <vector>
using i64 = long long;

template<class AbelianMonoid, class Ope, const AbelianMonoid& Ide>
struct fenwick_tree {
  using value_type = AbelianMonoid;

  i64 n;
  std::vector<value_type> node;
  Ope ope;

  fenwick_tree(i64 n_): n(n_), node(n + 1, Ide) {}
  fenwick_tree(const std::vector<value_type>& init): n(init.size()), node(n + 1, Ide) {
    for(i64 i = 0;i < init.size(); i++) node[i + 1] = init[i];
    for(i64 i = 1;i < n;i++) node[i + (i & -i)] = ope(node[i + (i & -i)], node[i]);
  }
  void modify(i64 i, value_type x) {
    i++;
    while(i <= n) {
      node[i] = ope(node[i], x);
      i += (i & -i);
    }
  }
  // [0, i)
  value_type sum(i64 i) const {
    value_type ret = Ide;
    while(i > 0) {
      ret = ope(ret, node[i]);
      i -= i & (-i);
    }
    return ret;
  }
};
