/*
 N個の正整数からなる集合 A={a1,a2,…,aN}があります。 太郎君と次郎君が次のゲームで勝負します。

最初に、K個の石からなる山を用意します。 二人は次の操作を交互に行います。 先手は太郎君です。
Aの元 xをひとつ選び、山からちょうど x個の石を取り去る。
先に操作を行えなくなった人が負けです。 二人が最適に行動すると仮定したとき、どちらが勝つかを判定してください。
*/

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

vector<i64> grn(101010, -1);

i64 N, K;
vector<i64> a;

int main() {
  cin >> N >> K;
  a.resize(N);
  for(int i = 0;i < N;i++) cin >> a[i];
  for(i64 k = 0;k <= K;k++) {
    set<i64> st;
    for(auto x: a) {
      if(k - x >= 0) st.insert(grn[k - x]);
    }
    grn[k] = K + 1;
    for(int i = 0;i <= K + 1;i++) {
      if(!st.count(i)) {
        grn[k] = i;
        break;
      }
    }
  }
  if(grn[K]) {
    cout << "First" << endl;
  }
  else {
    cout << "Second" << endl;
  }
}
