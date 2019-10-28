# Multiple Transform

約数幇助と呼ばれる. [参考 - 高速ゼータ変換の約数版](http://noshi91.hatenablog.com/entry/2018/12/27/121649)

### Spec

`i`について, `i`を約数に持つ`j`(上位集合)の`a[j]`の総和を求める. \\( O(N \log{\log N}) \\).  
`inverse multiple transform`と共に使うと`gcd`に関する畳み込みができる. こんな感じ

\\[ h(z) = \sum_{\gcd(x, y) = z} {f(x) * g(y)} \\]


### Code

```cpp
{{#include ../../../src/math/convolution/multiple_transform.hpp}}
```

