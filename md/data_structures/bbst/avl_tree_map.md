# AVL Tree Map

AVL TreeのMapバージョン

### Spec

- `struct node`
  - AVL Treeに載せるノードの型
  - `fix`に操作を書く.

- `at(key_value i)`
  - `i`の要素にアクセスする.

- `insert(key_value key, value_type val)`
  - `key`の要素を`val`にする

- `erase(key_value key)`
  - `key`の要素を消す

- `lower_bound(key_value key)`
  - `lower_bound`をしたときの要素を返す.
  - `.first`が`false`であるときは`lower_bound`はない.

- `nth_node(size_type i)`
  - `i`番目の要素を返す.

### Code

```cpp
{{#include ../../../src/data_structures/bbst/avl_tree_array.hpp}}
```
