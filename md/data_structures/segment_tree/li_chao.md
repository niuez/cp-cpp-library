# Li-Chao Line Add Tree

### Spec

- template argments
  - `class T`
    - Li-Chao Segment Treeで扱う型
    - `+, *, /`, 比較ができる必要がある
  - `const T ide`
    - Tの単位元(?)
    - 例えば, 最大値を返すLiChaoなら小さい数を入れておく
  - `class Compare = greater<T>`
    - 最大値or最小値

- `li_chao(T mi, T, ma)`
  - `[mi, ma]`の間の範囲を管理するLiChaoを構築する.

- `void add_line(T a, T b)`
  - `a * x + b`の直線を追加する
  - \\( O(\log L) \\)

- `T get(T x)`
  - `max{a_i * x + b_i}`を返す(Compareで`min`に変えられる)
  - \\( O(\log L) \\)

### Code

```cpp
{{#include ../../../src/data_structures/segment_tree/li_chao.hpp}}
```
