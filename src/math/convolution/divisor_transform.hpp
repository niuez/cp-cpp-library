#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

template <class T>
void divisor_transform(vector<T> &a) {
	int n = a.size();
	vector<bool> sieve(n, true);
	for (int p = 2; p < n; ++p) {
		if (sieve[p]) {
			for (int k = 1; k * p < n; ++k) {
				sieve[k * p] = false;
				a[k * p] += a[k];
			}
		}
	}
	for (int i = 0; ++i != n;) {
		a[i] += a[0];
	}
}

template <class T>
void inverse_divisor_transform(vector<T> &a) {
	int n = a.size();
	vector<bool> sieve(n, true);
	for (int i = 0; ++i != n;) {
		a[i] -= a[0];
	}
	for (int p = 2; p < n; ++p) {
		if (sieve[p]) {
			for (int k = (n - 1) / p; k != 0; --k) {
				sieve[k * p] = false;
				a[k * p] -= a[k];
			}
		}
	}
}

