# Link Cut Tree

動的木

パスのsumを計算したり, パスに対する作用を遅延伝搬できる.

### Spec

- `struct node`
  - link cut treeで扱うノードの構造体
  - この中に載せたいデータを載せる

- `fix(node * n)`
  - ノードの情報の再計算をする

- `reverse(node * n)`
  - 平衡二分木の反転
  - モノイドの演算順序が反転するのでその処理を書く
  - モノイドが可換であれば問題ない

- `lazy(node * n, i64 l)`
  - 遅延伝搬するときの演算
  - `expose(n); lazy(n, x)`をすると, `[root, n]`のパスに`x`を作用させることになる

- `push(node* n)`
  - 遅延伝搬
  - lazyを変えている場合はここも変更

- `expose(node* n)`
  - `n`をLink Cut Treeの根として, その木が`[root, n]`のパスをあらわすようになる

- `link(node* p, node* c)`
  - `p`を親, `c`を子として繋げる

- `cut(node* c)`
  - `c`の親とつながっている辺を切る

 - `evert(node* t)`
  - `t`を親にする

### Code

パス加算, パスsumを処理している

```cpp
{{#include ../../../src/data_structures/trees/link_cut_tree.hpp}}
```
