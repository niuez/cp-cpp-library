#include <set>
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

template<class T>
struct plane_to_line {
  using size_type = std::size_t;
  using coord_type = std::pair<T, T>;
  using value_type = T;

  std::vector<std::pair<coord_type, size_type>> elems;
  std::vector<size_type> yarray;
  std::vector<size_type> xstart;
  Compression<value_type> X, Y;

  plane_to_line(const std::vector<coord_type>& a): elems(a.size()) {
    for(size_type i = 0;i < a.size();i++) {
      elems[i] = { a[i], i };
      X.add(a[i].first);
      Y.add(a[i].second);
    }
    sort(std::begin(elems), std::end(elems));
    X.build();
    Y.build();

    xstart.resize(X.size() + 1);
    yarray.resize(elems.size());
    size_type x = 0;
    for(size_type i = 0;i < elems.size();i++) {
      if(!i || elems[i - 1].first.first < elems[i].first.first) {
        xstart[x++] = i;
      }
      yarray[i] = Y.comp(elems[i].first.second);
    }
    xstart[X.size()] = elems.size();
  }

  struct rangefreq_arg {
    size_type left;
    size_type right;
    size_type x;
    size_type y;
  };

  rangefreq_arg to_rangefreq(value_type xl, value_type xr, value_type yl, value_type yr) const {
    size_type cxl = xstart[X.comp(xl)];
    size_type cxr = xstart[X.comp(xr)];
    size_type cyl = Y.comp(yl);
    size_type cyr = Y.comp(yr);
    return rangefreq_arg { cxl, cxr, cyl, cyr };
  }
};
