#include <vector>
#include <numeric>
#include <set>

struct osa_k {
  using int_type = int;
  std::vector<int_type> min_fact;

  // O(NlogN)
  static std::vector<int_type> min_prime_factor(int n) {
    std::vector<int_type> res(n);
    std::iota(std::begin(res), std::end(res), 0);
    for(int i = 2; i * i < n; i++) {
      if(res[i] < i) continue;
      for(int j = i * i; j < n; j += i) {
        if(res[j] == j) res[j] = i;
      }
    }
    return res;
  }

  void build(int n) {
    min_fact = min_prime_factor(n);
  }

  // O(logN)
  std::vector<std::pair<int_type, int>> prime_factors(int n) const {
    std::vector<std::pair<int_type, int>> res;
    while(n > 1) {
      if(res.empty() || res.back().first != min_fact[n]) {
        res.push_back({ min_fact[n], 0 });
      }
      res.back().second++;
      n /= min_fact[n];
    }
    return res;
  }
  
  // The divisors are not sorted
  // O(logN + |divisors|)
  template<class F>
  void enumerate_divisors(int n, F f) const {
    std::vector<std::pair<int_type, int>> prime_facts = prime_factors(n);
    if(prime_facts.empty()) {
      f(1);
      return;
    }
    std::vector<int> cnt(prime_facts.size());
    std::vector<int> acc(prime_facts.size(), 1);
    while(true){
      f(acc.front());
      int i = 0;
      for(; i < prime_facts.size(); i++) {
        if((cnt[i]++) == prime_facts[i].second) {
          cnt[i] = 0;
        }
        else {
          acc[i] *= prime_facts[i].first;
          break;
        }
      }
      if(i == prime_facts.size()) {
        break;
      }
      while(i --> 0) {
        acc[i] = acc[i + 1];
      }
    }
  }
};
