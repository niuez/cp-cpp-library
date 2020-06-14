#include <vector>

/**
 * 
 * # Zeta and Mobius Transform
 *
 * ## superset_zeta_transform
 *
 * 上位集合に対して累積するゼータ変換
 *
 * \\[ g(S) := \sum_{T \supseteq S} f(T) \\]
 *
 **/
template<class T>
void superset_zeta_transform(int n, std::vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (!((s >> k) & 1)) {
        a[s] += a[s ^ (1 << k)];
      }
    }
  }
}

/**
 * ## subset_zeta_transform
 *
 * 下位集合に対して累積するゼータ変換
 *
 * \\[ g(S) := \sum_{T \subseteq S} f(T) \\]
 *
 **/
template<class T>
void subset_zeta_transform(int n, std::vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (((s >> k) & 1)) {
        a[s] += a[s ^ (1 << k)];
      }
    }
  }
}

/**
 * ## superset_mobius_transform
 *
 * 上位集合に対してのメビウス変換
 *
 * \\[ g(S) := \sum_{T \supseteq S} (-1) ^ {|T| - |S|} f(T) \\]
 *
 **/
template<class T>
void superset_mobius_transform(int n, std::vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (!((s >> k) & 1)) {
        a[s] -= a[s ^ (1 << k)];
      }
    }
  }
}

/**
 * ## subset_zeta_transform
 *
 * 下位集合に対してのメビウス変換
 *
 * \\[ g(S) := \sum_{T \subseteq S} (-1) ^ {|S| - |T|} f(T) \\]
 *
 **/
template<class T>
void subset_mobius_transform(int n, std::vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (((s >> k) & 1)) {
        a[s] -= a[s ^ (1 << k)];
      }
    }
  }
}
