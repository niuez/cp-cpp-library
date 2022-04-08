#include <vector>

template<class F>
std::vector<F> berlekamp_massey(const std::vector<F>& s) {
  int n = s.size();
  std::vector<F> b { F(1) };
  std::vector<F> c { F(1) };
  F y(1);
  int shift = 0;
  for(int len = 0; len < n; len++) {
    shift++;
    F x(0);
    for(int i = 0; i < c.size(); i++) {
      x += c[i] * s[len - i];
    }
    if(x == F(0)) { continue; }
    std::vector<F> old_c = c;
    F freq = x / y; c.resize(std::max(c.size(), b.size() + shift), F(0));
    for(int i = 0; i < b.size(); i++) {
      c[i + shift] -= freq * b[i];
    }
    if(old_c.size() < c.size()) {
      b = std::move(old_c);
      y = x;
      shift = 0;
    }
  }
  std::vector<F> ans(c.size() - 1);
  for(int i = 1; i < c.size(); i++) {
    ans[i - 1] = -c[i];
  }
  return ans;
}
