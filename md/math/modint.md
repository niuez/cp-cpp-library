# modint

xxで割ったあまりを求めよで使える構造体

### Spec

- template
  - `i64 M`
    - \\( \mod M \\)の剰余環

- `modint(const i64 x = 0)`
  - \\( x \mod M \\)で初期化

- `value()`
  - \\( x \mod M \\)を返す

- `pow(i64 r)`
  - \\( x^r \mod M \\)を返す

- `+, -, *, /`


### Code

```cpp
{{#include ../../src/math/modint.hpp}}
```
