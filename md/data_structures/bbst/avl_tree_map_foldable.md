# AVL Tree Map Foldable

AVL TreeのMapバージョンでモノイドを処理できるようにした.

### Spec

- `operation()`
  - モノイドの演算を決める

- `identity()`
  - モノイドの単位元を決める

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

- `fold(key_value left, key_value right)`
  - `[left, right)`に対してfoldをする.

### Code

```cpp
{{#include ../../../src/data_structures/bbst/avl_tree_foldable.hpp}}
```
