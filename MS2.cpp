#include<bits/stdc++.h>
using namespace std;



typedef pair<int,int> p;

bool compare(p &a,p &b){
	return (a.second < b.second || (a.second == b.second && a.first < b.first));
}

vector<int> solve(vector<int> &a){
	int n = a.size();
	unordered_map<int,int> mp;
	for(int i=0;i<n;i++) mp[a[i]]++;
	vector<p> temp;
	for(auto i : mp) temp.push_back({i.first,i.second});
	sort(temp.begin(),temp.end(),compare);
	vector<int> ans;
	for(int i=0;i<temp.size();i++){
		int cnt = temp[i].second;
		for(int j=0;j<cnt;j++) ans.push_back(temp[i].first);
	}
	return ans;
}

int main()
{
	int n;
	cin>>n;
	vector<int> a(n);
	for(int i=0;i<n;i++) cin>>a[i];
	vector<int> ans = solve(a);
	for(int i=0;i<ans.size();i++) cout<<ans[i]<<" ";
	return 0;
}