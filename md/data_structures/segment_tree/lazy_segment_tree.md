# Lazy Segment Tree
### Spec

- `using T = ...`
  - Segment Treeに載せる型

- `using L = ...`
  - 遅延伝搬させる作用素

- `T t_ide()`
  - Tの単位元を返す

- `L l_ide()`
  - Lの単位元を返す

- `T ope(const T& a, const T& b)`
  - Tの演算を指定する

- `L lazy_ope(const L& a, const L& b)`
  - Tの演算を指定する

- `T effect(const T& t, const L& l, const i64 len)`
  - 長さ`len`の区間の総和`t`に作用`l`をする

- `lazy_segment_tree(const vector<T>& init)`
  - `init`からLazy Segment Treeを構築する
  -  \\( O(n) \\)

- `update(i64 a, i64 b, L lx)`
  - `[a, b)`番目の要素に作用`lx`をする
  -  \\( O(\log n) \\)

- `sum(i64 a, i64 b)`
  - `[a, b)`の総和を求める
  -  \\( O(\log n) \\)

### Code

```cpp
{{#include ../../../src/data_structures/segment_tree/lazy_segment_tree.hpp}}
```

