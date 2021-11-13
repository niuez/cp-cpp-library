#include <tuple>
#include <vector>
std::tuple<long long, long long, long long> ext_gcd(long long a, long long b) {
  if(b == 0ll) {
    return std::tuple<long long, long long, long long>{ a, 1ll, 0ll };
  }
  else {
    auto [g, q, p] = ext_gcd(b, a%b);
    return std::tuple<long long, long long, long long>{ g, p, q-a/b*p };
  }
  ;
}
std::tuple<long long, long long> chinese_remainder_theorem(std::vector<std::tuple<long long, long long>> const& tms) {
  long long r = 0ll;
  long long M = 1ll;
  for(auto [t, m]: tms) {
    auto [g, p, q] = ext_gcd(M, m);
    if((t-r)%g != 0ll) {
      return std::tuple<long long, long long>{ 0ll, -1ll };
    }
    ;
    long long tmp = (t-r)/g*p%(m/g);
    r = r+M*tmp;
    M = M*(m/g);
  };
  return std::tuple<long long, long long>{ (r%M+M)%M, M };
}
