#include <vector>

template<class T>
struct Compression {
  using size_type = std::size_t;

  std::vector<T> v;

  Compression(){}
  void add(const T& t) { v.push_back(t); }
  void build() {
    sort(begin(v), end(v));
    v.erase(unique(begin(v), end(v)), end(v));
  }
  size_type comp(const T& x) const {
    return lower_bound(begin(v), end(v), x) - begin(v);
  }
  size_type size() const { return v.size(); }
};
