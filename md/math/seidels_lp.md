# Seidel's LP


# Seidel's LP algorithm
- LPを解くアルゴリズム
- LP(線形計画問題)とは, 以下のように記述できる.

\\[
\begin{align}
\text{maximize} \ & c^T x \\\\
\text{subject to} \ & A x \leq b \\\\
                 \ & x \geq 0
\end{align}
\\]

- この \\( x \\) を返す.
- 計算量 \\( O(d! m) ( d \text{は次元数}, m \text{は条件数}) \\)

## Arguments 

`mat`は, \\( (A \ b) \\)である.

`bounds`には, \\( x \\)のとる下界と上界を与える.

## Memo

`eps`を変える必要があるかもしれない.


### Code

```cpp
{{#include ../../src/math/seidels_lp.hpp}}
```
