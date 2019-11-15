# Compression

座標圧縮をする.

### Spec

- `add(T x)`
  - 要素`x`を追加する

- `build()`
  - 準備

- `comp(T x)`
  - 座圧した結果を返す

- `comp(vector<T> x)`
  - まとめて座圧
  - `x = comp(std::move(x))`すると楽です.

### Code


```cpp
{{#include ../../src/tech/compression.hpp}}
```
