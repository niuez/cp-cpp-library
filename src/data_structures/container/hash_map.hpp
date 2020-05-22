#include <vector>
#include <cstdint>
#include <set>
#include <iostream>

template<class Key, class Value, class Hash>
struct hash_map {
  using size_type = std::size_t;
  using key_type = Key;
  using value_type = Value;
  using pair_type = std::pair<key_type, value_type>;

  enum class State: std::uint8_t {
    INACTIVE,
    ACTIVE,
    FILLED
  };

  Hash hashf;

  std::vector<State> st;
  std::vector<pair_type> bck;
  size_type mask;
  size_type prode;
  size_type sz;
  size_type min_elem;
  hash_map():
    mask(0), prode(-1), sz(0), min_elem(0) {
  }

  size_type find_empty(const key_type& key) {
    size_type h = hashf(key);
    for(size_type delta = 0;;delta++) {
      size_type i = (h + delta) & mask;
      if(st[i] != State::FILLED) {
        if(prode < delta) prode = delta;
        return i;
      }
    }
  }

  size_type find_filled(const key_type& key) {
    if(sz == 0) return size_type(-1);
    size_type h = hashf(key);
    for(size_type delta = 0; delta <= prode; delta++) {
      size_type i = (h + delta) & mask;
      if(st[i] == State::FILLED) {
        if(bck[i].first == key) {
          return i;
        }
      }
      else if(st[i] == State::INACTIVE) {
        return size_type(-1);
      }
    }
    return size_type(-1);
  }

  size_type find_or_allocate(const key_type& key) {
    size_type h = hashf(key);
    size_type hole = size_type(-1);
    size_type delta = 0;
    for(; delta <= prode; delta++) {
      size_type i = (h + delta) & mask;
      if(st[i] == State::FILLED) {
        if(bck[i].first == key) return i;
      }
      else if(st[i] == State::INACTIVE) return i;
      else {
        if(hole == size_type(-1)) {
          hole = i;
        }
      }
    }
    if(hole != size_type(-1)) return hole;
    for(; ; delta++) {
      size_type i = (h + delta) & mask;
      if(st[i] != State::FILLED) {
        prode = delta;
        return i;
      }
    }
  }

  void reserve(int next_cnt) {
    size_type required_cnt = next_cnt + (next_cnt >> 1) + 1;
    if(required_cnt > bck.size()) {
      next_cnt = 4;
      while(next_cnt < required_cnt) next_cnt <<= 1;
    }
    else if(next_cnt <= bck.size() / 4) {
      next_cnt = std::max(4, (int)bck.size() / 2);
    }
    else {
      return;
    }
    std::vector<State> old_st(next_cnt, State::INACTIVE);
    std::vector<pair_type> old_bck(next_cnt);

    std::swap(old_st, st);
    std::swap(old_bck, bck);
    mask = next_cnt - 1;
    sz = 0;
    prode = 0;
    min_elem = next_cnt - 1;


    for(size_type pos = 0; pos < old_bck.size(); pos++) {
      if(old_st[pos] == State::FILLED) {
        size_type i = find_empty(old_bck[pos].first);
        st[i] = State::FILLED;
        bck[i] = std::move(old_bck[pos]);
        min_elem = std::min(min_elem, i);
        sz += 1;
      }
    }
  }

  void insert(const key_type& key, const value_type& val) {
    reserve(sz + 1);
    size_type i = find_or_allocate(key);
    if(st[i] != State::FILLED) {
      st[i] = State::FILLED;
      bck[i] = pair_type(key, val);
      min_elem = std::min(min_elem, i);
      sz++;
    }
    else {
      bck[i] = pair_type(key, val);
    }
  }

  bool erase(const key_type& key) {
    size_type i = find_filled(key);
    if(i == size_type(-1)) {
      return false;
    }
    else {
      st[i] = State::ACTIVE;
      bck[i].~pair_type();
      sz--;
      return true;
    }
  }

  pair_type* get(const key_type& key) {
    size_type i = find_filled(key);
    if(i == size_type(-1)) {
      return nullptr;
    }
    else {
      return &bck[i];
    }
  }

  pair_type* get_or_insert(const key_type& key, const value_type val) {
    reserve(sz + 1);
    size_type i = find_or_allocate(key);
    if(st[i] != State::FILLED) {
      st[i] = State::FILLED;
      bck[i] = pair_type(key, val);
      min_elem = std::min(min_elem, i);
      sz++;
    }
    return &bck[i];
  }

  pair_type get_and_erase(const key_type& key) {
    size_type i = find_filled(key);
    st[i] = State::ACTIVE;
    pair_type p = std::move(bck[i]);
    sz--;
    return p;
  }


  template<class Func>
  void search_all(Func func) {
    for(size_type i = min_elem; i < bck.size(); i++) {
      if(st[i] == State::FILLED) {
        min_elem = i;
        size_type res = func(bck[i]);
        if(res & 0b10) {
          st[i] = State::ACTIVE;
          bck[i].~pair_type();
          sz--;
        }
        if(res & 0b01) {
           return;
        }
      }
    }
  }

  size_type size() const { return sz; }
};


struct Hashu32 {
  std::uint32_t operator()(std::uint32_t key) {
    int c2=0x27d4eb2d; // a prime or an odd constant
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);
    return key;
  }
};

struct Hashu64 {
  std::size_t operator()(std::uint64_t key) {
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21; // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return (int) key;
  }

};
