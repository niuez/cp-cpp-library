# Dynamic Segment Tree
### Spec

- `using T = ...`
  - Dynamic Segment Treeに載せる型

- `T ope(const T& a, const T& b)`
  - Tの演算を指定する

- `T ide()`
  - Tの単位元を返す

- `dynamic_segment_tree(const i64 n)`
  - `[ide; n]`の列を扱うDynamic Segment Treeを構築する
  - \\( O(1) \\)

- `update(i64 i, T x)`
  - `i`番目の要素を`x`にする
  -  \\( O(\log n) \\)

- `get(i64 l, i64 r)`
  - `[l, r)`の総和を求める
  -  \\( O(\log n) \\)

### Code

```cpp
{{#include ../../../src/data_structures/segment_tree/dynamic_segment_tree.hpp}}
```
