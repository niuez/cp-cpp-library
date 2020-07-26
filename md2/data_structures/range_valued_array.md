# Code

```cpp
struct range_valued_array {
  using key_type = int;
  using value_type = long long;
  using iterator_type = std::map<key_type, value_type>::iterator;

  std::map<key_type, value_type> mp;
  key_type len;

  range_valued_array(): len(0) {}

  void resize_with(key_type new_len, const value_type& val) {
    if(new_len == 0) mp.clear();
    else if(len < new_len) mp[new_len] = val;
    else if(len > new_len) {
      auto iter = inner_split(new_len);
      mp.erase(iter, mp.end());
    }
    len = new_len;
  }

  iterator_type inner_split(key_type pos) {
    if(pos == 0) return mp.begin();
    auto iter = mp.lower_bound(pos);
    if(iter->first == pos) return ++iter;
    mp[pos] = iter->second;
    return iter;
  }

  iterator_type range_set_value(key_type l, key_type r, const value_type& val) {
    if(l == r) return mp.end();
    auto liter = inner_split(l);
    auto riter = inner_split(r);
    mp.erase(liter, riter);
    return mp.insert({r, val}).first;
  }

  iterator_type begin() {
    return mp.begin();
  }
  iterator_type end() {
    return mp.end();
  }

  void debug_print() {
    std::cout << "[";
    for(auto p: mp) {
      std::cout << p.first << ": " << p.second << ", ";
    }
    std::cout << "]" << std::endl;
  }
};
```
