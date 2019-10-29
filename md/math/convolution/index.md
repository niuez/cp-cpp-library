# Convolution

畳み込み [参考 - 約数集合でのゼータ変換・メビウス変換的なやつと畳み込み](http://kazuma8128.hatenablog.com/entry/2018/07/29/231819)

## 集合の畳み込みについて

\\(A \subseteq B\\) のとき, \\( B \\)は\\( A \\)の上位集合という. また\\( A \\)は\\( B \\)の下位集合という.

上位集合について和を求める変換して互いをかけ合わせて逆変換をすると, 積集合(And)の個数が求められる.  
下位集合について和を求める変換して互いをかけ合わせて逆変換をすると, 和集合(Or)の個数が求められる.

例) `multiple_transform`(`i`について, `i`を約数に持つ`j`の`a[j]`の総和を求める --> 上位集合)をして互いをかけ合わせて逆変換すると, 積集合(`gcd`についての畳み込み)がの個数が求められる.

## Transforms

- [Multiple Transform](./multiple_transform.md)
- [Divisor Transform](./divisor_transform.md)
- [Fast Fourier Transform](./fast_fourier_transform.md)
