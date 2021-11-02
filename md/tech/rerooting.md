# Rerooting

- `T` ... 答え
- `E` ... 辺の型
- `U` ... 辺から伸ばしたときの値, 可換モノイド
- `To` ... `E -> V` 頂点番号の取得
- `Adj` ... `T x E -> U` 辺から伸ばす
- `Ope` ... `U x U -> U` モノイドの値
- `Merge` ... `U x V -> T` モノイドと頂点から答えを導く

### Code

```cpp
{{#include ../../src/tech/rerooting.hpp}}
```
