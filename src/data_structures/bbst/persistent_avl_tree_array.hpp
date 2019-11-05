#include <bits/stdc++.h>
#include <cassert>
using i64 = long long;

class persistent_avl_array {
  using T = char;

  struct node;

  using node_type = typename std::shared_ptr<const node>;
  using node_ref = const node_type&;
  using value_type = T;
  using value_ref = const value_type&;
  using size_type = i64;
  using split_node_type = std::pair<node_type, node_type>;

  static size_type size(node_ref n) {
    if(n) return n->sz;
    else return 0;
  }

  static size_type height(node_ref n) {
    if(n) return n->he;
    else return 0;
  }

  struct node {
    
    using node_type = std::shared_ptr<const node>;
    using node_ref = const node_type&;
    using value_type = T;
    using value_ref = const value_type&;
    using size_type = i64;

    value_type val;
    node_type ch[2];
    size_type sz;
    size_type he;

    node(value_ref val, node_ref left, node_ref right)
      : val(val) {
        sz = 1 + size(left) + size(right);
        he = 1 + std::max(height(left), height(right));
        assert(std::abs(height(left)-height(right))<=1);
        ch[0] = left;
        ch[1] = right;
      }
  };

  static node_type balance(value_ref rootval, node_ref a, node_ref b) {
    if(height(a) - height(b) == 2) {
      if(height(a->ch[0]) - height(a->ch[1]) == -1)
        return std::make_shared<const node>(
            a->ch[1]->val,
            std::make_shared<const node> (a->val, a->ch[0], a->ch[1]->ch[0]),
            std::make_shared<const node> (rootval, a->ch[1]->ch[1], b)
            );
      else
        return std::make_shared<const node>(
            a->val, 
            a->ch[0],
            std::make_shared<const node> (rootval, a->ch[1], b)
            );
    }
    else if(height(a) - height(b) == -2) {
      if(height(b->ch[0]) - height(b->ch[1]) == 1)
        return std::make_shared<const node> (
            b->ch[0]->val,
            std::make_shared<const node> (rootval, a, b->ch[0]->ch[0]),
            std::make_shared<const node> (b->val, b->ch[0]->ch[1], b->ch[1])
            );
      else
        return std::make_shared<const node> (
            b->val,
            std::make_shared<const node>(rootval, a, b->ch[0]),
            b->ch[1]
            );
    }
    else
      return std::make_shared<const node>(rootval, a, b);
  }

  static node_ref back(node_ref node) {
    if(node->ch[1]) return back(node->ch[1]);
    else return node;
  }

  static node_type push_back(node_ref node, value_ref val) {
    if(!node) return std::make_shared<const struct node>(val, nullptr, nullptr);
    else if(node->ch[1]) return merge(node->val, node->ch[0], push_back(node->ch[1], val));
    else return merge(node->val, node->ch[0], std::make_shared<const struct node>(val, nullptr, nullptr));
  }

  static node_type pop_back(node_ref node) {
    if(node->ch[1]) return merge(node->val, node->ch[0], pop_back(node->ch[1]));
    else return node->ch[0];
  }

  static node_type merge_1(value_ref rootval, node_ref dst, node_ref src) {
    if(height(dst) - height(src) <= 1)
      return std::make_shared<const node>(rootval, dst, src);
    else
      return balance(dst->val, dst->ch[0], merge_1(rootval, dst->ch[1], src));
  }

  static node_type merge_0(value_ref rootval, node_ref dst, node_ref src) {
    if(height(dst) - height(src) <= 1)
      return std::make_shared<const node>(rootval, src, dst);
    else
      return balance(dst->val, merge_0(rootval, dst->ch[0], src), dst->ch[1]);
  }

  static node_type merge(value_ref rootval, node_ref left, node_ref right) {
    if(height(left) >= height(right)) 
      return merge_1(rootval, left, right);
    else
      return merge_0(rootval, right, left);
  }

  static split_node_type split(node_ref node, size_type i) {
    if(i == 0)
      return split_node_type(node_type(), node);
    else if(i <= size(node->ch[0])) {
      auto sp = split(node->ch[0], i);
      return split_node_type(sp.first, merge(node->val, sp.second, node->ch[1]));
    }
    else {
      auto sp = split(node->ch[1], i - size(node->ch[0]) - 1);
      return split_node_type(merge(node->val, node->ch[0], sp.first), sp.second);
    }
  }

  static node_ref at(node_ref node, size_type i) {
    if(i == size(node->ch[0])) return node;
    else if(i < size(node->ch[0])) return at(node->ch[0], i);
    else return at(node->ch[1], i - 1 - size(node->ch[0]));
  }

  node_type root;

public:
  
  using split_array_type = std::pair<persistent_avl_array, persistent_avl_array>;

  persistent_avl_array() {}
  persistent_avl_array(node_ref r) : root(r) {}

  persistent_avl_array merge(persistent_avl_array other) const {
    if(!root) return other;
    else return persistent_avl_array(merge(back(root)->val, pop_back(root), other.root));
  }

  split_array_type split(size_type i) const {
    auto p = split(root, i);
    return { persistent_avl_array(p.first), persistent_avl_array(p.second) };
  }

  persistent_avl_array push_back(value_ref val) const {
    return persistent_avl_array(push_back(root, val));
  }

  size_type len() const { return size(root); }

  value_ref at(size_type i) const { return at(root, i)->val; }
};

using namespace std;

void debug_cout(const persistent_avl_array& arr) {
    for(int i = 0;i < arr.len();i++){
      cout << arr.at(i);
    }
    cout << endl;
}

int main() {
  std::ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 m;
  cin >> m;
  string s;
  cin >> s;
  i64 n;
  cin >> n;

  persistent_avl_array arr;

  for(char c: s) { arr = arr.push_back(c);}

  for(int i = 0;i < n;i++) {
    i64 a, b, c;
    cin >> a >> b >> c;
    
    auto cpy = arr.split(b).first.split(a).second;
    auto spl = arr.split(c);
    

    arr = spl.first.merge(cpy).merge(spl.second);
    if(arr.len() > m) arr = arr.split(m).first;
  }

  for(int i = 0;i < arr.len();i++){
    cout << arr.at(i);
  }
  cout << endl;
}

