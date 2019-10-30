# MP (Morris Pratt)

[文字列の頭良い感じの線形アルゴリズムたち](https://snuke.hatenablog.com/entry/2014/12/01/235807)

### Spec

文字列S[0, i-1]のprefixとsuffixが最大何文字一致しているかを \\( O(|S|) \\)で求める.


#### Example
```
aabaabaaa
_010123452
```

### Code

```cpp
{{#include ../../src/string/mp.hpp}}
```
