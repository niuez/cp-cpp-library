# Black Box Linear Algebra


ベクトルと行列の掛け算に\\(T(n)\\)時間かかるとします.

### 最小多項式

\\(O(n^2 + n T(n))\\)

```cpp
{{#include ../../src/math/black_box_linear_algebra/minimal_polynomial.hpp}}
```

### 行列式

\\(O(n^2 + n T(n))\\)

```cpp
{{#include ../../src/math/black_box_linear_algebra/fast_determinant.hpp}}
```

### 行列累乗

\\(A^k b\\)を求める. `fast_kitamasa`を用いて\\(O(N^3 + N \log N \log k)\\)

```cpp
{{#include ../../src/math/black_box_linear_algebra/fast_matrix_pow.hpp}}
```
