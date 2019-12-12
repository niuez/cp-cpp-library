# Partially Persistent Union Find

## Spec

- 時刻`t`は単調増加である必要がある.

- `(constructor)`
  - `n`要素のPartially Persistent Union Findを構築する.

- `unite(i64 t, i64 x, i64 y)`
  - 2要素を時刻`t`で結ぶ.
  - 戻り値は親となった根

- `find(i64 t, i64 x)`
  - 時刻`t`のときの要素`x`の根を返す.

- `size(i64 t, i64 x)`
  - 時刻`t`のときの要素`x`の属する集合の大きさを返す.

## Code

```cpp
{{#include ../../../src/data_structures/union_find/persistent_union_find.hpp }}
```
