# Grundy Number

### Spec

- `grn[(ゲームの状態)]`が`0` -> その状態での手番の人が負ける
- `grn[(ゲームの状態)]`が`0`以外 -> その状態での手番の人が勝つ

### Code

このコードでは, 取れる個数に制限がついているNimの勝敗を計算している.

```cpp
{{#include ../../src/other/grundy.hpp}}
```
