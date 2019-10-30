# Splay Tree Array

列を管理するSplay Tree

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

- `split(i64 i)`
  - `[0, i) / [i, ..)`に分ける

- `merge(splay_array&& arr)`
  - mergeする

- `reverse()`
  - 列全体を反転させる

- `update(i64 i, T t)`
  - `i`番目の要素を`t`に変更する.


### Code

```cpp
{{#include ../../../src/data_structures/bbst/splay_tree_array.hpp}}
```
