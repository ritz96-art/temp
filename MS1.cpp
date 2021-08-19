#include<bits/stdc++.h>
using namespace std;

int dfs(vector<vector<int>> &adj,vector<int> &p,int i,vector<bool> &visit){
	visit[i] = true;
	for(auto v : adj[i]){
		if(!visit[v]){
			p[i] += dfs(adj,p,v,visit);
		}
	}
	return p[i];
}

vector<int> solve(vector<vector<int>> &adj,vector<int> &p,int n){
	vector<bool> visit(n+1,false);
	for(int i=1;i<=n;i++){
		if(!visit[i]){
			dfs(adj,p,i,visit);
		}
	}
	vector<int> ans;
	queue<int> q;
	q.push(1);
	while(!q.empty()){
		int u = q.front();
		q.pop();
		ans.push_back(p[u]);
		for(auto j : adj[u]) q.push(j);
	}
	return ans;
}
	

int main()
{
	int n;
	cin>>n;
	vector<int> points(n+1);
	for(int i=1;i<=n;i++) cin>>points[i];
	int m;
	cin>>m;
	int num = pow(2,m) -1;
	vector<int> node(num);
	for(int i=0;i<num;i++) cin>>node[i];
	vector<vector<int>> adj(n+1);
	for(int i=0;i<num;i++){
		int u = node[i];
		if(u != -1){
			if(2*i + 1 < num){
				int v1 = node[2*i + 1];
				if(v1 != -1) adj[u].push_back(v1);
			}
			if(2*i + 2 < num){
				int v2 = node[2*i + 2];
				if(v2 != -1) adj[u].push_back(v2);
			}
		}
	}
	vector<int> ans = solve(adj,points,n);
	for(int i=0;i<ans.size();i++) cout<<ans[i]<<" ";
}
	