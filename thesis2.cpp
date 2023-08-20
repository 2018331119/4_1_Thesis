//suffix array solution of adition of new string of ASAP problem
// building complexity of suffix array is O(n)
#include<bits/stdc++.h>
using namespace std;
//Be careful lcp[i] means lcp of sorted suffix i and i+1
//chnage value of LG according to string length
//for separator use any character which is greater than '$',or can replace $ with '@' 
// 0 based indexing

void induced_sort(const vector<int> &vec, int val_range, vector<int> &SA, const vector<bool> &sl, const vector<int> &lms_idx) {
  vector<int> l(val_range, 0), r(val_range, 0);
  for (int c : vec) {
    if (c + 1 < val_range) ++l[c + 1];
    ++r[c];
  }
  partial_sum(l.begin(), l.end(), l.begin());
  partial_sum(r.begin(), r.end(), r.begin());
  fill(SA.begin(), SA.end(), -1);
  for (int i = lms_idx.size() - 1; i >= 0; --i)
    SA[--r[vec[lms_idx[i]]]] = lms_idx[i];
  for (int i : SA)
    if (i >= 1 && sl[i - 1]) {
      SA[l[vec[i - 1]]++] = i - 1;
    }
  fill(r.begin(), r.end(), 0);
  for (int c : vec)
    ++r[c];
  partial_sum(r.begin(), r.end(), r.begin());
  for (int k = SA.size() - 1, i = SA[k]; k >= 1; --k, i = SA[k])
    if (i >= 1 && !sl[i - 1]) {
      SA[--r[vec[i - 1]]] = i - 1;
    }
}

vector<int> SA_IS(const vector<int> &vec, int val_range) {
  const int n = vec.size();
  vector<int> SA(n), lms_idx;
  vector<bool> sl(n);
  sl[n - 1] = false;
  for (int i = n - 2; i >= 0; --i) {
    sl[i] = (vec[i] > vec[i + 1] || (vec[i] == vec[i + 1] && sl[i + 1]));
    if (sl[i] && !sl[i + 1]) lms_idx.push_back(i + 1);
  }
  reverse(lms_idx.begin(), lms_idx.end());
  induced_sort(vec, val_range, SA, sl, lms_idx);
  vector<int> new_lms_idx(lms_idx.size()), lms_vec(lms_idx.size());
  for (int i = 0, k = 0; i < n; ++i)
    if (!sl[SA[i]] && SA[i] >= 1 && sl[SA[i] - 1]) {
      new_lms_idx[k++] = SA[i];
    }
  int cur = 0;
  SA[n - 1] = cur;
  for (size_t k = 1; k < new_lms_idx.size(); ++k) {
    int i = new_lms_idx[k - 1], j = new_lms_idx[k];
    if (vec[i] != vec[j]) {
      SA[j] = ++cur;
      continue;
    }
    bool flag = false;
    for (int a = i + 1, b = j + 1;; ++a, ++b) {
      if (vec[a] != vec[b]) {
        flag = true;
        break;
      }
      if ((!sl[a] && sl[a - 1]) || (!sl[b] && sl[b - 1])) {
        flag = !((!sl[a] && sl[a - 1]) && (!sl[b] && sl[b - 1]));
        break;
      }
    }
    SA[j] = (flag ? ++cur : cur);
  }
  for (size_t i = 0; i < lms_idx.size(); ++i)
    lms_vec[i] = SA[lms_idx[i]];
  if (cur + 1 < (int)lms_idx.size()) {
    auto lms_SA = SA_IS(lms_vec, cur + 1);
    for (size_t i = 0; i < lms_idx.size(); ++i) {
      new_lms_idx[i] = lms_idx[lms_SA[i]];
    }
  }
  induced_sort(vec, val_range, SA, sl, new_lms_idx);
  return SA;
}
vector<int> suffix_array(const string &s, const int LIM = 128) {
  vector<int> vec(s.size() + 1);
  copy(begin(s), end(s), begin(vec));
  vec.back() = '$';
  auto ret = SA_IS(vec, LIM);
  ret.erase(ret.begin());
  return ret;
}
struct SuffixArray {
  int n;
  string s;
  vector<int> sa, rank, lcp;
  static const int LG = 22;
  vector<vector<int>> t;
  vector<int> lg;
  SuffixArray() {}
  SuffixArray(string _s) {
    n = _s.size();
    s = _s;
    sa = suffix_array(s);
    rank.resize(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    costruct_lcp();
    prec();
    build();
  }
  void costruct_lcp() {
    int k = 0;
    lcp.resize(n - 1, 0);
    for (int i = 0; i < n; i++) {
      if (rank[i] == n - 1) {
        k = 0;
        continue;
      }
      int j = sa[rank[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k])  k++;
      lcp[rank[i]] = k;
      if (k)  k--;
    }
  }
  void prec() {
    lg.resize(n, 0);
    for (int i = 2; i < n; i++) lg[i] = lg[i / 2] + 1;
  }
  void build() {
    int sz = n - 1;
    t.resize(sz);
    for (int i = 0; i < sz; i++) {
      t[i].resize(LG);
      t[i][0] = lcp[i];
    }
    for (int k = 1; k < LG; ++k) {
      for (int i = 0; i + (1 << k) - 1 < sz; ++i) {
        t[i][k] = min(t[i][k - 1], t[i + (1 << (k - 1))][k - 1]);
      }
    }
  }
  int query(int l, int r) {
  //assert(r<n-1); // minimum of lcp[l], ..., lcp[r]
    int k = lg[r - l + 1];
    return min(t[l][k], t[r - (1 << k) + 1][k]);
  }
}; 
int n;
std::vector<string> string_list;
std::vector<vector<int>> ans_grid;
std::vector<int> str, saa;
int tot_len;
string S;
void find_solution() {
	n = string_list.size();
	ans_grid.resize(n);
	for(int i = 0; i < n; i++) ans_grid[i].resize(n);
	SuffixArray SA(S);
	auto fill_the_row = [&](int row, int ind) {
		//cout << row << "\n";
		for(int j = ind - 1; j >= 0; j--) {
			int rmq = SA.query(j, ind - 1);
			if(rmq >= saa[SA.sa[j]]) {
				int col = str[SA.sa[j]];
				ans_grid[row][col] = max(ans_grid[row][col], saa[SA.sa[j]]);
			}
		}
		for(int j = ind; j < tot_len - 1; j++) {
			int rmq = SA.query(ind, j);
			if(rmq < string_list[row].size()) break;
			int col = str[SA.sa[j + 1]];
			ans_grid[row][col] = max(ans_grid[row][col], saa[SA.sa[j + 1]]);
		}
	};

	for(int i = 0; i < tot_len; i++) {
		int a = SA.sa[i];
		int id = str[a];
		if(a == 0 || S[a - 1] == '@') {
			fill_the_row(id, i);
		}
	}
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			//cout << ans_grid[i][j] << " ";
		} //cout << "\n";
	}
}
void solve() {
	string c; 
	auto start = chrono::steady_clock::now();
	while(cin >> c) {
		c += '@';
		int cur_len = c.size();
		int a = string_list.size();
		for(int i = tot_len, j = cur_len  - 1; i < tot_len + cur_len; i++, j--) 
			str.push_back(a), saa.push_back(j);
		tot_len += cur_len;
		string_list.push_back(c);
		S += c;
		find_solution();
	}
	//find_solution();
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
