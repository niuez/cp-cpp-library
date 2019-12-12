#include <memory>
#include <array>
#include <cassert>
#include <iostream>

template<class Key, class Monoid>
class avl_tree_map {
  public:
    using key_type = Key;
    using value_type = Monoid;
    using size_type = std::size_t;
    using height_type = long long int;
  protected:
    class node;
    using node_type = std::unique_ptr<class node>;

    static size_type size(const node_type& node) {
      if(!node) return 0;
      else return node->size();
    }
    static height_type height(const node_type& node) {
      if(!node) return 0;
      else return node->height();
    }

    static value_type identity() {
      return 0;
    }

    static value_type operation(const value_type& a, const value_type& b) {
      return a + b;
    }

    class node {
      private:
        key_type ky;
        value_type val;
        size_type sz;
        height_type hei;
        std::array<node_type, 2> chi;
      public:

        value_type fold;

        node(key_type key, value_type val): ky(std::move(key)), val(std::move(val)), fold(identity()), sz(1), hei(1), chi() { fix(); }
        void fix() {
          fold = val;
          if(chi[0]) fold = operation(chi[0]->fold, fold);
          if(chi[1]) fold = operation(fold, chi[1]->fold);
          sz = avl_tree_map::size(chi[0]) + avl_tree_map::size(chi[1]) + 1;
          hei = std::max(avl_tree_map::height(chi[0]), avl_tree_map::height(chi[1])) + 1;
        }
        void push() {}
        node_type cut(size_type dir) {
          push();
          node_type nn = std::move(chi[dir]);
          this->fix();
          return std::move(nn);
        }
        void set(node_type dir_node, size_type dir) {
          push();
          chi[dir] = std::move(dir_node);
          this->fix();
        }
        size_type size() { push(); return sz; }
        height_type height() { push(); return hei; }
        height_type diff() { push(); return avl_tree_map::height(chi[0]) - avl_tree_map::height(chi[1]); }
        value_type& value() { push(); return val; }
        const key_type& key() { push(); return ky; }
        node_type& child(size_type dir) { return chi[dir]; }
    };


    static node_type rotate(node_type x, size_type dir) {
      node_type y = x->cut(1 - dir);
      node_type b = y->cut(dir);
      x->set(std::move(b), 1 - dir);
      y->set(std::move(x), dir);
      return std::move(y);
    }

    static node_type balance(node_type node) {
      if(node->diff() == 2) {
        if(node->child(0)->diff() == -1) {
          auto ch = node->cut(0);
          node->set(rotate(std::move(ch), 0), 0);
        }
        return rotate(std::move(node), 1);
      }
      else if(node->diff() == -2) {
        if(node->child(1)->diff() == 1) {
          auto ch = node->cut(1);
          node->set(rotate(std::move(ch), 1), 1);
        }
        return rotate(std::move(node), 0);
      }
      else return std::move(node);
    }

    static node_type insert(node_type node, key_type key, value_type val) {
      if(!node) {
        return node_type(new class node(std::move(key), std::move(val)));
      }
      else if(node->key() == key) {
        node->value() = val;
        node->fix();
        return std::move(node);
      }
      else if(node->key() < key) {
        auto right = node->cut(1);
        node->set(insert(std::move(right), std::move(key), std::move(val)), 1);
        return balance(std::move(node));
      }
      else {
        auto left = node->cut(0);
        node->set(insert(std::move(left), std::move(key), std::move(val)), 0);
        return balance(std::move(node));
      }
    }

    static std::pair<node_type, node_type> deepest_node(node_type node, size_type dir) {
      auto ch = node->cut(dir);
      if(ch) {
        auto pp = deepest_node(std::move(ch), dir);
        node_type deepest_node, dirn;
        deepest_node = std::move(pp.first);
        dirn = std::move(pp.second);
        node->set(std::move(dirn), dir);
        pp.first = std::move(deepest_node);
        pp.second = balance(std::move(node));
        return std::move(pp);
      }
      else {
        auto rn = node->cut(1 - dir);
        std::pair<node_type, node_type> pp;
        pp.first = std::move(node);
        pp.second = std::move(rn);
        return pp;
      }
    }

    static node_type erase(node_type node, key_type key) {
      if(!node) { return std::move(node); }
      else if(node->key() == key) {
        node_type& left = node->child(0);
        node_type& right = node->child(1);
        if(!left) {
          return node->cut(1);
        }
        else {
          node_type deepest, ln;
          std::tie(deepest, ln) = deepest_node(std::move(left), 1);
          deepest->set(std::move(ln), 0);
          deepest->set(std::move(right), 1);
          return balance(std::move(deepest));
        }
      }
      else if(node->key() < key) {
        auto right = node->cut(1);
        node->set(erase(std::move(right), std::move(key)), 1);
        return balance(std::move(node));
      }
      else {
        auto left = node->cut(0);
        node->set(erase(std::move(left), std::move(key)), 0);
        return balance(std::move(node));
      }
    }

    static node_type& at(node_type& node, key_type key) {
      if(!node) assert(false);
      else if(node->key() == key) return node;
      else if(node->key() < key) return at(node->child(1), std::move(key));
      else return at(node->child(0), std::move(key));
    }

    static node_type& lower_bound(node_type& node, key_type key) {
      if(!node) return node;
      else if(key < node->key()) {
        auto& ans = lower_bound(node->child(0), std::move(key));
        if(ans) return ans;
        else return node;
      }
      else {
        return lower_bound(node->child(1), std::move(key));
      }
    }

    static node_type& nth_node(node_type& node, size_type i) {
      if(size(node->child(0)) == i) return node;
      else if(size(node->child(0)) < i) return nth_node(node->child(1), i - size(node->child(0)) - 1);
      else return nth_node(node->child(0), i);
    }

    static value_type left_fold(node_type& node, key_type left) {
      if(!node) { return identity(); }
      if(node->key() < left) { return left_fold(node->child(1), std::move(left)); }
      else { 
        value_type R = identity();
        if(node->child(1)) R = node->child(1)->fold;
        return operation(left_fold(node->child(0), std::move(left)), operation(node->value(), R));
      }
    }

    static value_type right_fold(node_type& node, key_type right) {
      if(!node) { return identity(); }
      if(!(node->key() < right)) { return right_fold(node->child(0), std::move(right)); }
      else {
        value_type L = identity();
        if(node->child(0)) L = node->child(0)->fold;
        return operation(L, operation(node->value(), right_fold(node->child(1), std::move(right))));
      }
    }

    static value_type go_fold(node_type& node, key_type left, key_type right) {
      if(!node) return identity();
      if(left <= node->key() && node->key() < right) {
        return operation(left_fold(node->child(0), std::move(left)), operation(node->value(), right_fold(node->child(1), std::move(right))));
      }
      else if(node->key() < left) {
        return go_fold(node->child(1), std::move(left), std::move(right));
      }
      else {
        return go_fold(node->child(0), std::move(left), std::move(right));
      }
    }

    node_type root;
    avl_tree_map(node_type&& root): root(std::move(root)) {}
  public:
    avl_tree_map(): root() {}
    avl_tree_map(avl_tree_map&& tree): root(std::move(tree.root)) {}
    avl_tree_map& operator=(avl_tree_map&& tree) {
      root = std::move(tree.root);
      return *this;
    }

    value_type& at(key_type key) {
      return at(root, std::move(key))->value();
    }

    void insert(key_type key, value_type val) {
      root = insert(std::move(root), std::move(key), std::move(val));
    }

    void erase(key_type key) {
      root = erase(std::move(root), std::move(key));
    }

    std::pair<bool, std::pair<key_type, value_type>> lower_bound(key_type key) {
      auto& node = lower_bound(root, std::move(key));
      if(node) {
        return { true, { node->key(), node->value() } };
      }
      else {
        return { false, { key_type(), value_type() } };
      }
    }

    std::pair<key_type, value_type> nth_node(size_type i) {
      auto node = nth_node(root, i);
      return { node->key(), node->value() };
    }

    size_type size() {
      if(!root) return 0;
      return root->size();
    }

    value_type fold(key_type left, key_type right) {
      return go_fold(root, left, right);
    }
};
