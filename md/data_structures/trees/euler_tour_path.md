# EulerTour Path

EulerTourのパスを処理するバージョン  
パスとして求められるのは, 上から下に降りるようなパスだけなので, 任意のパスを扱うときはLCAをしないといけない.  
扱える要素には, 可逆性, 可換性(?)が必要.

### Spec

- `euler_tour_path(i64 n)`
  - `n`頂点の木を構築する準備

- `add_edge(i64 u, i64 v)`
  - 頂点`u`と`v`を結ぶ辺を追加する

- `start_tour(i64 r)`
  - `r`を根としてEulerTourを行う
  - \\( O(n) \\)

- `edge_in(i64 v)`
  - 頂点`v`に入る辺のindexを返す
  - \\( O(1) \\)

- `edge_out(i64 v)`
  - 頂点`v`に出る辺のindexを返す
  - \\( O(1) \\)

- `path_range(i64 u, i64 v)`
  - 頂点`u`から降りて頂点`v`に辿るパスの範囲を返す.
  - \\( O(1) \\)

### Code

```cpp
{{#include ../../../src/data_structures/trees/euler_tour_path.hpp}}
```
