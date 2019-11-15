# Persistent AVL Tree Array

列を永続的に管理するAVL Tree

### Spec

- `struct node`
  - Persistent AVL Treeに載せるノードの型

  - `node(value_ref rootval, node_ref a, node_ref b)`
    - 永続化ではノードを作るときだけ`fix`する
    - ここにfix操作を書く.

- `split(size_type i)`
  - `[0, i) / [i, ..)`に分けた木を返す

- `merge(persistent_avl_array&& arr)`
  - mergeした木を返す.

- `at(size_type i)`
  - `i`番目の要素にアクセスする.

- `push_back(val)`
  - 列の一番うしろに要素を追加する.

### Code

```cpp
{{#include ../../../src/data_structures/bbst/persistent_avl_tree_array.hpp}}
```
