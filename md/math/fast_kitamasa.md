# Fast Kitamasa

線形漸化式\\(a_i = c_1 * a_{i - 1} + \cdots + c_d * a_{i - d}\\)があるとき、\\(c_0 = -1\\)として`fast_kitamasa`にかけると、
\\(a_n = b_0 * a_{d - 1} + \cdots + b_{d - 1} * a_0\\)となる\\(b\\)を\\(O(d \log d \log n)\\)で求める

```cpp
{{#include ../../src/math/fast_kitamasa.hpp}}
```
