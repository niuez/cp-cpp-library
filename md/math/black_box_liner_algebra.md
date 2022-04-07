# Black Box Liner Algebra


ベクトルと行列の掛け算に\\(T(n)\\)時間かかるとします.

### 最小多項式

\\(O(n^2 + n T(n))\\)

```cpp
{{#include ../../src/math/black_box_liner_algebra/minimal_polynomial.hpp}}
```

### 行列式

\\(O(n^2 + n T(n))\\)

```cpp
{{#include ../../src/math/black_box_liner_algebra/fast_determinant.hpp}}
```
