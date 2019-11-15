# Heavy Light Decomposition

静的木

木のパスや部分木のクエリを処理できる.

### Spec

- `HeavyLightDecomposition(i64 n)`
  - 頂点数`n`で初期化

- `add_edge(i64 u, i64 v)`
  - 頂点`u`と`v`をつなぐ

- `build(i64 r)`
  - `r`を根としてHLDecompを行う

- `sequence()`
  - HLDecompしたときのオイラーツアーの配列.

- `path(i64 a, i64 b, bool edge)`
  - パスの列を返す.
  - `edge = true`で辺に対するパスを返す.
  - \\( O(\log n) \\)

- `subtree(i64 v, bool edge)`
  - 部分木の列を返す.
  - `edge = true`で辺に対するパスを返す.
  - \\( O(1) \\)

### Code

```cpp
{{#include ../../../src/data_structures/trees/heavy_light_decomposition.hpp}}
```
