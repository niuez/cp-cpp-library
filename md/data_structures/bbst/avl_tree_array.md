# AVL Tree Array

列を管理するAVL Tree

### Spec

- `struct node`
  - AVL Treeに載せるノードの型
  - `fix`に操作を書く

- `split(size_type i)`
  - `[0, i) / [i, ..)`に分けた木を返す

- `merge(persistent_avl_array&& arr)`
  - mergeした木を返す.

- `at(size_type i)`
  - `i`番目の要素にアクセスする.

- `set(size_type i, value_type val)`
  - `i`番目の要素を`val`にする

### Code

```cpp
{{#include ../../../src/data_structures/bbst/avl_tree_array.hpp}}
```
