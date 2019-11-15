# Pairing Heap

マージ可能で高速なHeap

### Spec

- `top()`
  - 最小値を返す
  - \\( O(1) \\)

- `pop()`
  - 最小値の要素を削除する
  - \\( O(\log n) \\)

- `push(const T& x)`
  - 要素`x`を追加する
  - \\( O(1) \\)

- `meld(h)`
  - ヒープ`h`とマージする
  - \\( O(1) \\)

### Code

```cpp
{{#include ../../../src/data_structures/heap/pairing_heap.hpp}}
```
