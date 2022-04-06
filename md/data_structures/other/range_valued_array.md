# Range Valued Array

`[, r)`のmapとして管理する範囲set可能な配列

### Spec

- `(constructor)(value_type init_value)`
  - `init_value`: 全域をこの値で初期化する
- `iterator_type range_set_value(key_type l, ley_type r, const value_type& val)`
  - `[l, r)`を`val`で初期化する
- `operator[](const key_type& k)`
  - インデックス`k`の値を返す
  

### Code

```cpp
{{#include ../../../src/data_structures/range_valued_array.hpp}}
```
