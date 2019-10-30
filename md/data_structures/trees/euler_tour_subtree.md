# EulerTour Subtree

EulerTourの部分木を処理するバージョン  
木を列に落とし込んだときの部分木の範囲がわかるので, [Segment Tree](../segment_tree/segment_tree.md)と合わせて使うといい.

LCAをET Subtree + RMQで求める場合はコメントアウトしてある部分を使う.

### Spec

- `eulertour_subtree(i64 n)`
  - `n`頂点の木を構築する準備

- `add_edge(i64 u, i64 v)`
  - 頂点`u`と`v`を結ぶ辺を追加する

- `start_tour(i64 r)`
  - `r`を根としてEulerTourを行う
  - \\( O(n) \\)

- `subtree_range(i64 v)`
  - 頂点`v`の部分木に対応する範囲を返す
  - \\( O(1) \\)

- `vertex(i64 v)`
  - 頂点`v`に対応するindexを返す.
  - \\( O(1) \\)

### Code

```cpp
{{#include ../../../src/data_structures/trees/euler_tour_subtree.hpp}}
```
