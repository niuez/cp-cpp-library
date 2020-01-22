#include <vector>
#include <iostream>

template<class T>
const std::vector<int>& sa_is(std::vector<T> s, int k) {
  int N = s.size();
  static std::vector<int> sa;
  static std::vector<int> cnt;
  sa.resize(N + 1);

  if(N == 0) return sa;

  for(auto& c: s) c++;
  s.push_back(0);
  k++;

  std::vector<bool> iss(N + 1);
  std::vector<int> lms;
  std::vector<int> is_lms(N + 1, -1);
  std::vector<int> bin(k + 1);

  iss[N] = true;
  bin[1]++;
  for(int i = N; i --> 0; ) {
    if(s[i] == s[i + 1])
      iss[i] = iss[i + 1];
    else
      iss[i] = s[i] < s[i + 1];
    if(!iss[i] && iss[i + 1]) {
      is_lms[i + 1] = lms.size();
      lms.push_back(i + 1);
    }
    bin[s[i] + 1]++;
  }

  for(int i = 1;i <= k;i++)
    bin[i] += bin[i - 1];

  auto induce = [&](const std::vector<int>& lms) {
    sa.assign(N + 1, -1);
    cnt.assign(k, 0);

    for(int i = 0;i < lms.size();i++) {
      int x = lms[i];
      sa[bin[s[x] + 1] - 1 - cnt[s[x]]] = x;
      cnt[s[x]]++;
    }

    cnt.assign(k, 0);
    for(int i = 0;i <= N;i++) {
      int x = sa[i] - 1;
      if(x >= 0 && !iss[x]) {
        sa[bin[s[x]] + cnt[s[x]]] = x;
        cnt[s[x]]++;
      }
    }

    cnt.assign(k, 0);
    for(int i = N + 1;i --> 0;) {
      int x = sa[i] - 1;
      if(x >= 0 && iss[x]) {
        sa[bin[s[x] + 1] - 1 - cnt[s[x]]] = x;
        cnt[s[x]]++;
      }
    }
  };

  induce(lms);


  if(lms.size() >= 2) {
    int M = lms.size();
    int li = 0;
    std::vector<int> rec_lms(M);
    for(auto x: sa) {
      if(is_lms[x] != -1) rec_lms[li++] = x;
    }
    int rec_n = 1;
    std::vector<int> rec_s(M);
    rec_s[M - 1 - is_lms[rec_lms[1]]] = 1;
    for(int i = 2;i < M;i++) {
      int xl = rec_lms[i];
      int yl = rec_lms[i - 1];
      int xr = lms[is_lms[xl] - 1];
      int yr = lms[is_lms[yl] - 1];
      if(xr - xl != yr - yl)
        rec_n++;
      else {
        while(xl <= xr) {
          if(s[xl] != s[yl]) {
            rec_n++;
            break;
          }
          xl++;
          yl++;
        }
      }
      rec_s[M - 1 - is_lms[rec_lms[i]]] = rec_n;
    }


    sa_is(std::move(rec_s), rec_n + 1);
    li = M;
    for(int i = 1;i < M + 1;i++) {
      rec_lms[--li] = lms[M - 1 - sa[i]];
    }
    induce(rec_lms);
  }

  return sa;
}

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

template<class T>
class wavelet_matrix {
  using Integer = T;
  using integer_type = Integer;
  using size_type = std::size_t;


  size_type depth;
  size_type len;
  std::vector<bitvector> mat;
  std::vector<size_type> spl;

public:

  wavelet_matrix(): depth(0), len(0) {}
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

  // counting elements that equal to x in range [left, right)
  size_type rank_x(size_type left, size_type right, integer_type x) const {
    for(size_type d = depth; d-- > 0;) {
      size_type k = ((x >> d) & 1);
      if(k) {
        left = mat[d].rank(left, k) + spl[d];
        right = mat[d].rank(right, k) + spl[d];
      }
      else {
        left = mat[d].rank(left, k);
        right = mat[d].rank(right, k);
      }
    }
    return right - left;
  }

  // sorted(arr[left..right])[i]
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

  struct rank_result {
    size_type le;
    size_type eq;
    size_type mo;
  };

  // couting elements that less than x, equal to x, and more than x in range [left, right)
  rank_result rank_less_eq_more(size_type left, size_type right, integer_type x) const {
    size_type le = 0, mo = 0;
    for(size_type d = depth; d --> 0;) {
      size_type k = (x >> d) & 1;
      size_type l = mat[d].rank(left, 1);
      size_type r = mat[d].rank(right, 1);
      if(k == 0) {
        mo += r - l;
        left -= l;
        right -= r;
      }
      else {
        le += (right - left) - (r - l);
        left = l + spl[d];
        right = r + spl[d];
      }
    }
    return rank_result { le, right - left, mo };
  }

  size_type rangefreq(size_type left, size_type right, integer_type x, integer_type y, integer_type l, size_type d) const {
    integer_type r = l + (1 << d);
    if(x <= l && r <= y) {
      return right - left;
    }
    else if(y <= l || r <= x) {
      return 0;
    }
    else {
      d--;
      size_type lr = mat[d].rank(left, 1);
      size_type rr = mat[d].rank(right, 1);
      return
        rangefreq(left - lr, right - rr, x, y, l, d) +
        rangefreq(lr + spl[d], rr + spl[d], x, y, l + (1 << d), d);
    }
  }

  size_type rangefreq(size_type left, size_type right, integer_type x, integer_type y) const {
    return rangefreq(left, right, x, y, 0, depth);
  }
};

template<class T>
struct burrows_wheeler {
  std::vector<int> sa;
  std::vector<T> L;
  std::vector<int> C;
  wavelet_matrix<T> wm;
  burrows_wheeler(const std::vector<T>& s, int k, int de): sa(sa_is(s, k)), L(s.size() + 1), C(k + 1) {
    for(int i = 0;i < sa.size();i++) {
      if(sa[i] > 0) L[i] = s[sa[i] - 1];
      else L[i] = 0;
    }
    for(int i = 0;i < s.size();i++) {
      C[s[i] + 1]++;
    }
    C[0]++;
    for(int i = 0; i < k; i++) {
      C[i + 1] += C[i];
    }
    wm = wavelet_matrix<T>(L, de);
  }
  int match(const std::string& p) {
    int s = 0;
    int e = sa.size();
    for(int i = p.size();i --> 0;) {
      s = wm.rank_x(0, s, p[i]) + C[p[i]];
      e = wm.rank_x(0, e, p[i]) + C[p[i]];
      if(s >= e) break;
    }
    return e - s;
  }
};
