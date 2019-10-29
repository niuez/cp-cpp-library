# Fast Fourier Transform

### Spec

離散フーリエ変換(discrete Fourier transformation)を\\( O(n \log n) \\)で行う.

誤差厳しい, 整数ならNTT使おう(誤差をなくすようにFFTするのもあるらしい)

### Code

```cpp
{{#include ../../../src/math/convolution/fast_fourier_transform.hpp}}
```
