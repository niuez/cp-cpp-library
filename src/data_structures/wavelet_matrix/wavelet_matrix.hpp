#include <cstdint>
#include <set>
#include <vector>
#include <iostream>

using i64 = long long;

class bitvector {
  using bit_type = std::uint_least64_t;
  using size_type = std::size_t;
  static constexpr size_type wordsize = 64;
  
  std::vector<bit_type> bit;
  std::vector<size_type> sum;
  
public:

  bitvector() : bit(), sum() {} 
  bitvector(const size_type size)
    : bit(size / wordsize + 1, 0), sum(size / wordsize + 1, 0) {}

  void set(const size_type i) {
    bit[i / wordsize] |= static_cast<bit_type>(1) << (i % wordsize);
  }
  void build() {
    for (size_type i = 1; i < bit.size(); i++) {
      sum[i] = sum[i - 1] + __builtin_popcountll(bit[i - 1]);
    }
  }

  size_type at(const size_type i) const {
    return bit[i / wordsize] >> (i % wordsize);
  }

  // count of ones in [0, i)
  size_type rank(const size_type i) const {
    return sum[i / wordsize]
      + __builtin_popcountll(bit[i / wordsize] & (static_cast<bit_type>(1) << (i % wordsize)) - 1);
  }

  // count of ones in [0, i)
  size_type rank(const size_type i, const size_type b) const {
    size_type ans = sum[i / wordsize]
      + __builtin_popcountll(bit[i / wordsize] & (static_cast<bit_type>(1) << (i % wordsize)) - 1);
    if(b) return ans;
    else return i - ans;
  }
};

class wavelet_matrix {
  using Integer = i64;
  using integer_type = Integer;
  using size_type = std::size_t;


  const size_type depth;
  const size_type len;
  std::vector<bitvector> mat;
  std::vector<size_type> spl;

public:

  wavelet_matrix(const std::vector<integer_type>& arr, size_type de)
    : depth(de),
      len(arr.size()),
      mat(std::vector<bitvector>(depth, bitvector(arr.size()))),
      spl(std::vector<size_type>(depth, 0)) {
        std::vector<size_type> idx(len);
        std::vector<size_type> left(len), right(len);
        for(size_type i = 0;i < len;i++) idx[i] = i;
        for(size_type d = depth; d-- > 0;) {
          size_type l = 0, r = 0;
          
          for(size_type i = 0; i < len; i++) {
            size_type k = (arr[idx[i]] >> d) & 1;
            if(k) right[r++] = idx[i], mat[d].set(i);
            else left[l++] = idx[i];
          }
          mat[d].build();
          spl[d] = l;
          swap(idx, left);
          for(size_type i = 0; i < r; i++) idx[i + l] = right[i];
        }
      }

  integer_type at(size_type i) const {
    integer_type x = static_cast<integer_type>(0);
    for(size_type d = depth; d-- > 0;) {
      size_type k = mat[d].at(i);
      x |= (static_cast<integer_type>(k) << d);
      i = mat[d].rank(i, k) + spl[d] * k;
    }
    return x;
  }

  size_type rank_x(size_type left, size_type right, integer_type x) const {
    for(size_type d = depth; d-- > 0;) {
      size_type k = ((x >> d) & 1);
      left = mat[d].rank(left, k) + spl[d] * k;
      right = mat[d].rank(right, k) + spl[d] * k;
    }
    return right - left;
  }

  integer_type quantile(size_type left, size_type right, size_type i) const {
    integer_type x = static_cast<integer_type>(0);
    for(size_type d = depth; d-- > 0;) {
      size_type cnt = mat[d].rank(right, 0) - mat[d].rank(left, 0);
      size_type k = (i < cnt) ? 0 : 1;
      x |= (k << d);
      left = mat[d].rank(left, k) + spl[d] * k;
      right = mat[d].rank(right, k) + spl[d] * k;
    }
    return x;
  }

  size_type greater_eq(size_type left, size_type right, integer_type x) const {
    size_type cnt = 0;
    for(size_type d = depth; d-- > 0;) {
      size_type k = ((x >> d) & 1);
      size_type rl = mat[d].rank(left, 1);
      size_type rr = mat[d].rank(right, 1); 
      if(k == 0) {
        cnt += rr - rl;
        left -= rl;
        right -= rr;
      }
      else {
        left = rl + spl[d];
        right = rr + spl[d];
      }
    }
    return cnt + right - left;
  }

  size_type rangefreq(size_type left, size_type right, integer_type x, integer_type y) const {
    return greater_eq(left, right, x) - greater_eq(left, right, y);
  }

  size_type greater(size_type left, size_type right, integer_type x) const {
    size_type cnt = 0;
    for(size_type d = depth; d-- > 0;) {
      size_type k = ((x >> d) & 1);
      size_type rl = mat[d].rank(left, 1);
      size_type rr = mat[d].rank(right, 1); 
      if(k == 0) {
        cnt += rr - rl;
        left -= rl;
        right -= rr;
      }
      else {
        left = rl + spl[d];
        right = rr + spl[d];
      }
    }
    return cnt;
  }
};
