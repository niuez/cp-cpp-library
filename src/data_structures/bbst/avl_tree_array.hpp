#include <memory>
#include <array>

template<class T>
class avl_tree_array {
  public:
    using value_type = T;
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

    class node {
      private:
        value_type val;
        size_type sz;
        height_type hei;
        std::array<node_type, 2> chi;
      public:

        node(value_type val): val(std::move(val)), sz(1), hei(1), chi() { fix(); }
        void fix() {
          sz = avl_tree_array::size(chi[0]) + avl_tree_array::size(chi[1]) + 1;
          hei = std::max(avl_tree_array::height(chi[0]), avl_tree_array::height(chi[1])) + 1;
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
        height_type diff() { push(); return avl_tree_array::height(chi[0]) - avl_tree_array::height(chi[1]); }
        value_type& value() { push(); return val; }
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

    static node_type merge_dir(node_type dst, node_type root, node_type src, size_type dir) {
      if(std::abs(height(dst) - height(src)) <= 1) {
        root->set(std::move(src), dir);
        root->set(std::move(dst), 1 - dir);
        return std::move(root);
      }
      else {
        node_type ch = dst->cut(dir);
        if(ch) {
          ch = merge_dir(std::move(ch), std::move(root), std::move(src), dir);
          dst->set(std::move(ch), dir);
          return balance(std::move(dst));
        }
        else {
          root->set(std::move(src), dir);
          root = balance(std::move(root));
          dst->set(std::move(root), dir);
          return balance(std::move(dst));
        }
      }
    }

    static node_type merge(node_type left, node_type right) {
      if(!left) { return std::move(right); }
      else if(!right) { return std::move(left); }
      else if(height(left) >= height(right)) {
        node_type deep_left, src;
        std::tie(deep_left, src) = deepest_node(std::move(right), 0);
        return merge_dir(std::move(left), std::move(deep_left), std::move(src), 1);
      }
      else {
        node_type deep_right, src;
        std::tie(deep_right, src) = deepest_node(std::move(left), 1);
        return merge_dir(std::move(right), std::move(deep_right), std::move(src), 0);
      }
    }

    static std::pair<node_type, node_type> split(node_type node, size_type i) {
      if(i == node->size()) { return std::pair<node_type, node_type>(std::move(node), node_type()); }
      auto left = node->cut(0);
      auto right = node->cut(1);
      if(i < size(left)) {
        node_type sp_left, sp_right;
        std::tie(sp_left, sp_right) = split(std::move(left), i);
        node_type nright;
        if(right) {
          nright = merge_dir(std::move(right), std::move(node), std::move(sp_right), 0);
        }
        else {
          nright = merge(std::move(sp_right), std::move(node));
        }
        return std::pair<node_type, node_type>(std::move(sp_left), std::move(nright));
      }
      else if(i == size(left)) {
        return std::pair<node_type, node_type>(std::move(left), merge(std::move(node), std::move(right)));
      }
      else {
        node_type sp_left, sp_right;
        std::tie(sp_left, sp_right) = split(std::move(right), i - size(left) - 1);
        node_type nleft;
        if(left) {
          nleft = merge_dir(std::move(left), std::move(node), std::move(sp_left), 1);
        }
        else {
          nleft = merge(std::move(node), std::move(sp_left));
        }
        return std::pair<node_type, node_type>(std::move(nleft), std::move(sp_right));
      }
    }

    static node_type& at(node_type& node, size_type i) {
      if(size(node->child(0)) == i) return node;
      else if(size(node->child(0)) < i) return at(node->child(1), i - size(node->child(0)) - 1);
      else return at(node->child(0), i);
    }

    static void set(node_type& node, size_type i, value_type val) {
      if(size(node->child(0)) == i) {
        node->value() = std::move(val);
        node->fix();
      }
      else if(size(node->child(0)) < i) {
        set(node->child(1), i - size(node->child(0)) - 1, std::move(val));
        node->fix();
      }
      else {
        set(node->child(0), i, std::move(val));
        node->fix();
      }
    }

    node_type root;
    avl_tree_array(node_type&& root): root(std::move(root)) {}
  public:
    avl_tree_array(): root() {}
    avl_tree_array(T val): root(node_type(new class node(std::move(val)))) {}
    avl_tree_array(avl_tree_array&& tree): root(std::move(tree.root)) {}
    avl_tree_array& operator=(avl_tree_array&& tree) {
      root = std::move(tree.root);
      return *this;
    }
    template<class A>
      friend avl_tree_array<A> merge(avl_tree_array<A>&& t1, avl_tree_array<A>&& t2);
    template<class A>
      friend std::pair<avl_tree_array<A>, avl_tree_array<A>> split(avl_tree_array<A>&& t, std::size_t i);

    value_type& at(size_type i) {
      return at(root, i)->value();
    }

    void set(size_type i, value_type val) {
      set(root, i, std::move(val));
    }

    size_type size() {
      if(!root) return 0;
      return root->size();
    }
};

template<class T>
avl_tree_array<T> merge(avl_tree_array<T>&& t1, avl_tree_array<T>&& t2) {
  return avl_tree_array<T>(avl_tree_array<T>::merge(std::move(t1.root), std::move(t2.root)));
}

template<class T>
std::pair<avl_tree_array<T>, avl_tree_array<T>> split(avl_tree_array<T>&& t, std::size_t i) {
  auto rp = avl_tree_array<T>::split(std::move(t.root), i);
  return std::make_pair(avl_tree_array<T>(std::move(rp.first)), avl_tree_array<T>(std::move(rp.second)));
}
