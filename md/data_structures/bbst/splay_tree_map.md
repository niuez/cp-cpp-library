# Splay Tree Map

map型のSplay Tree

### Spec

- `struct node`
  - Splay Treeに載せるノードの型

- `fold(node* x)`
  - `fold`したいやつを変えたときはここを変える

- `fix(node* n)`
  - `fix`操作, foldの仕方をここで定義する

- `reverse(node* n)`
  - 反転操作, foldを反転する必要がある場合はここに.

- `push(node* x)`
  - 遅延伝搬させているときはここを変更

- `insert(Key key, T t)`
  - `{key, t}`を入れる, すでに存在する場合はupdate

- `erase(Key key)`
  - `{key, ..}のノードを削除する

- `nth_node(i64 n)`
  - `n`番目の要素の`Key`を返す


### Code

```cpp
{{#include ../../../src/data_structures/bbst/splay_tree_map.hpp}}
```
