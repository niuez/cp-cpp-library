# Divisor Transform

約数幇助と呼ばれる. [参考 - 高速ゼータ変換の約数版](http://noshi91.hatenablog.com/entry/2018/12/27/121649)

### Spec

`i`について, `i`の約数である`j`(下位集合)について`a[j]`の総和を求める. \\( O(N \log{\log N}) \\).  
`inverse divisor transform`と共に使うと`lcm`に関する畳み込みができる. こんな感じ

\\[  h(z) = \sum_{ \operatorname{lcm} (x, y) = z } { f(x) * g(y) } \\]


### Code

```cpp
{{#include ../../../src/math/convolution/divisor_transform.hpp}}
```

