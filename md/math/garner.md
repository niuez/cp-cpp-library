# Garner's Algorithm

FPSで使う

### Spec

\\(m_0, \cdots, m_{k-1} \\)が互いに素とする.  
\\( x < \prod{m_i} \\)を満たす\\( x \\)について, \\( x \mod m_i \\)がわかっている時,  
\\( O(k^2 + k \log m) \\)で \\( x \mod M \\) を求めることができる.

### Code

```cpp
{{#include ../../src/math/garner.hpp}}
```
