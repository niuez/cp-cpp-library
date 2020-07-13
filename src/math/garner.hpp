#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

i64 pow_mod(i64 x, i64 r, i64 mod) {
  i64 ans = 1;
  while(r) {
    if(r & 1) ans = (ans * x) % mod;
    r >>= 1;
    x = x * x % mod;
  }
  return ans;
}

i64 inv_mod(i64 x, i64 mod) {
  return pow_mod(x, mod - 2, mod);
}

i64 garner(const vector<i64> &x, vector<i64> mods, i64 mod) {
  mods.emplace_back(mod);
  vector<i64> coeffs(x.size() + 1, 1);
  vector<i64> constants(x.size() + 1, 0);
  for(i64 i = 0; i < x.size(); i++) {
    i64 v = (x[i] - constants[i]) * inv_mod(coeffs[i], mods[i]) % mods[i];
    if(v < 0) v += mods[i];
    for(i64 j = i + 1; j < x.size() + 1; j++) {
      constants[j] = (constants[j] + coeffs[j] * v) % mods[j];
      coeffs[j] = (coeffs[j] * mods[i]) % mods[j];
    }
  }
  return constants.back();
}

