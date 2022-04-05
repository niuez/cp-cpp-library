# Segment Tree
### Spec

- `using T = ...`
  - Segment Treeに載せる型

- `segment_tree(vector<T> init)`
  - `init`からSegment Treeを構築する
  -  \\( O(n) \\)

- `update(i64 i, T x)`
  - `i`番目の要素を`x`にする
  -  \\( O(\log n) \\)

- `sum(i64 l, i64 r)`
  - `[l, r)`の総和を求める
  -  \\( O(\log n) \\)

- `find_first(i64 l, F isok)`
  - isokを満たす`[l, x)`のsumの中でxが最小のもの
  - sumが区間の大きさについて単調である必要がある
  - ないなら-1を返す

### Code

```cpp
{{#include ../../../src/data_structures/segment_tree/segment_tree.hpp}}
```
