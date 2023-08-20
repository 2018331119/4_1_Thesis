// bruteforce solution of adition of new string of ASAP problem
#include<bits/stdc++.h>
using namespace std;
int n;
std::vector<string> string_list;
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
				if(string_list[i].substr(0, k) == string_list[j].substr(b - k,k)) {
					ans_grid[i][j] = k;
					break;
				}
			}
			//cout << ans_grid[i][j] << " ";
		} //cout << "\n";
	}
}
void solve() {
	string c; 
	auto start = chrono::steady_clock::now();
	while(cin >> c) {
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
