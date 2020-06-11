#include <vector>
#include <set>
#include <algorithm>

template<class T>
struct radix_heap {
  using key_type = unsigned;
  std::vector<std::pair<key_type, T>> bucket[33];
  key_type size, last;

  radix_heap(): size(0), last(0) {}
  bool empty() const { return size == 0; }
  inline int getbit(int a) {
    return a ? 32 - __builtin_clz(a) : 0;
  }
  void push(key_type key, const T& value) {
    size++;
    bucket[getbit(key ^ last)].emplace_back(key, value);
  }
  std::pair<key_type, T> pop() { 
    if(bucket[0].empty()) {
      int idx = 1;
      while(bucket[idx].empty()) idx++;
      last = std::min_element(std::begin(bucket[idx]), end(bucket[idx]))->first;
      for(auto& p: bucket[idx]) bucket[getbit(p.first ^ last)].emplace_back(p);
      bucket[idx].clear();
    }
    size--;
    auto ret = bucket[0].back();
    bucket[0].pop_back();
    return ret;
  }
};
