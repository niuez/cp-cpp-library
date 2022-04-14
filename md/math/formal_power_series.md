# Formal Power Series

### FPS

FPS 
- `resize`: リサイズ
- `resized`: リサイズしたFPSを返す
- `+=, -=, +, -`: 加減法、次数の大きい方にリサイズされる。
- `*, *=`: 定数倍
- `DFT dft(int n)`: 長さ`n`にリサイズして`dft`
- `inv(int n)`: \\(f * g \equiv 1 \pmod{x^n}\\)となるような\\(g\\)を返す
- `diff(int n)`: \\(f' \bmod{x^n}\\)
- `integral(int n)`: \\(\int f dx \bmod{x^n}\\)
- `log(int n)`: \\(\log f \bmod{x^n}\\)
- `exp(int n)`: \\(\exp f \bmod{x^n}\\)

DFT
- `+=, -=, +, -`: 加減法 線形性より
- `*, *=`: 定数倍 線形性より
- `*`: 畳み込み
- `idft(int n = -1)`: `idft`した後に長さ`n`でリサイズ 指定しなければそのまま

```cpp
{{#include ../../src/math/formal_power_series.hpp}}
```

### NTTをそのまま使うとき

```cpp
{{#include ../../src/math/fps_ntt_multiply.hpp}}
```

### 任意modで

```cpp
{{#include ../../src/math/fps_multiply_arb.hpp}}
```
