#include <map>
#include <iostream>

struct range_valued_array {
  using key_type = int;
  using value_type = long long;
  using iterator_type = std::map<key_type, value_type>::iterator;
  
  // [, r)
  std::map<key_type, value_type> mp;
  key_type start;
  value_type init_value;
  

  range_valued_array(key_type start = key_type(), value_type init_value = value_type()): start(start), init_value(init_value) {}

  // key of return iterator is `pos`
  iterator_type inner_split(key_type pos) {
    if(pos == start) return mp.begin();
    auto iter = mp.lower_bound(pos);
    if(iter->first == pos) return iter;
    mp[pos] = iter != mp.end() ? iter->second : init_value;
    return --iter;
  }

  iterator_type range_set_value(key_type l, key_type r, const value_type& val) {
    if(l == r) return mp.end();
    auto liter = inner_split(l);
    auto riter = inner_split(r);
    std::cout << liter->first << " " << riter->first << std::endl;
    mp.erase(++liter, riter);
    riter->second = val;
    return riter;
  }

  iterator_type begin() {
    return mp.begin();
  }
  iterator_type end() {
    return mp.end();
  }

  value_type& operator[](const key_type& k) {
    auto iter = mp.upper_bound(k);
    return iter == mp.end() ? init_value : iter->second;
  }

  void debug_print() {
    std::cout << "[";
    for(auto p: mp) {
      std::cout << p.first << ": " << p.second << ", ";
    }
    std::cout << "]" << std::endl;
  }
};
