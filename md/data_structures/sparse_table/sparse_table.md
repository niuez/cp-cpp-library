# Sparse Table

## Spec

- `ope`
  - 束の演算を定義する.

- `(constructor)`
  - Sparse Tableを構築する. \\( O(N \log N) \\)

- `query(i64 s, i64 t)`
  - `[s, t)`に対してクエリを処理する.

## Code

```cpp
{{ ../../../src/data_structures/sparse_table/sparse_table.hpp }}
```
