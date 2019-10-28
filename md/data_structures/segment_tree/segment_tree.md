# Segment Tree
### Spec

- `using T = ...`
  - Segment Treeに載せる型

- `T ope(const T& a, const T& b)`
  - Tの演算を指定する

- `T ide()`
  - Tの単位元を返す

- `segment_tree(vector<T> init)`
  - `init`からSegment Treeを構築する
  -  \\( O(n) \\)

- `update(i64 i, T x)`
  - `i`番目の要素を`x`にする
  -  \\( O(\log n) \\)

- `sum(i64 l, i64 r)`
  - `[l, r)`の総和を求める
  -  \\( O(\log n) \\)

### Code

```cpp
{{#include ../../../src/data_structures/segment_tree/segment_tree.hpp}}
```
