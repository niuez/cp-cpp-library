# Berlekamp-Massey

線形漸化式\\( a_i = c_0 * a_(i - 1) + c_1 * a_(i - 2) + ... \\)があって、数列\\(a\\)がわかっているときに、漸化式の係数\\(c\\)を\\(O(n^2)\\)で求める.

### Code

```cpp
{{#include ../../src/math/berlekamp_massey.hpp}}
```
