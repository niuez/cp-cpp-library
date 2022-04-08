# Numeric Theoretic Transform

### Spec

素数\\( m = 2^k + 1, 2^k >= n\\)として\\(F_m \\)剰余環上での離散フーリエ変換(discrete Fourier transformation)を\\( O(n \log n) \\)で行う  
`NTT_PRIMES`は[るまライブラリ](https://lumakernel.github.io/ecasdqina/math/FFT/NTT)からお借りしています

ほとんど単体でNTTを使うことはなさそう(FPSで使うね)

### Code

```cpp
{{#include ../../../src/math/convolution/number_theoretic_transform.hpp}}
```

### 4基底NTT

```cpp
{{#include ../../../src/math/convolution/ntt4.hpp}}
```
