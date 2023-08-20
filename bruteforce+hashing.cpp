// bruteforce solution of adition of new string of ASAP problem
// optimize the solution slightly using hashing when comparing two substring
#include<bits/stdc++.h>
using namespace std;
const int N = 1e6 + 9;

int power(long long n, long long k, const int mod) {
  int ans = 1 % mod;
  n %= mod;
  if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}

const int MOD1 = 127657753, MOD2 = 987654319;
const int p1 = 137, p2 = 277;
int ip1, ip2;
pair<int, int> pw[N], ipw[N];
void prec() {
  pw[0] =  {1, 1};
  for (int i = 1; i < N; i++) {
    pw[i].first = 1LL * pw[i - 1].first * p1 % MOD1;
    pw[i].second = 1LL * pw[i - 1].second * p2 % MOD2;
  }
  ip1 = power(p1, MOD1 - 2, MOD1);
  ip2 = power(p2, MOD2 - 2, MOD2);
  ipw[0] =  {1, 1};
  for (int i = 1; i < N; i++) {
    ipw[i].first = 1LL * ipw[i - 1].first * ip1 % MOD1;
    ipw[i].second = 1LL * ipw[i - 1].second * ip2 % MOD2;
  }

}
struct Hashing {
  int n;
  string s; // 0 - indexed
  vector<pair<int, int>> hs; // 1 - indexed
  Hashing() {}
  Hashing(string _s) {
    n = _s.size();
    s = _s;
    hs.emplace_back(0, 0);
    for (int i = 0; i < n; i++) {
      pair<int, int> p;
      p.first = (hs[i].first + 1LL * pw[i].first * s[i] % MOD1) % MOD1;
      p.second = (hs[i].second + 1LL * pw[i].second * s[i] % MOD2) % MOD2;
      hs.push_back(p);
    }
  }
  pair<int, int> get_hash(int l, int r) { // 1 - indexed
    assert(1 <= l && l <= r && r <= n);
    pair<int, int> ans;
    ans.first = (hs[r].first - hs[l - 1].first + MOD1) * 1LL * ipw[l - 1].first % MOD1;
    ans.second = (hs[r].second - hs[l - 1].second + MOD2) * 1LL * ipw[l - 1].second % MOD2;
    return ans;
  }
  pair<int, int> get_hash() {
    return get_hash(1, n);
  }
};
int n;
std::vector<string> string_list;
std::vector<Hashing> string_hash;
std::vector<vector<int>> ans_grid;
void find_solution() {
	n = string_list.size();
	ans_grid.resize(n);
	for(int i = 0; i < n; i++) ans_grid[i].resize(n);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			int a = min(string_list[i].size(), string_list[j].size());
			int b = string_list[j].size();
			for(int k = a; k >= 1; k--) {
				if(string_hash[i].get_hash(1, k) == string_hash[j].get_hash(b - k + 1, b)) {
					ans_grid[i][j] = k;
					break;
				}
			}
			//cout << ans_grid[i][j] << " ";
		} //cout << "\n";
	}
}
void solve() {
	prec();
	string c; 
	auto start = chrono::steady_clock::now();
	while(cin >> c) {
		Hashing h(c);
		string_hash.push_back(h);
		string_list.push_back(c);
		find_solution();
	}
	auto end = chrono::steady_clock::now();
	cout << "Elapsed time in milliseconds: "
    << chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << " ms" << endl;
}
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	solve();
	return 0; 
}
