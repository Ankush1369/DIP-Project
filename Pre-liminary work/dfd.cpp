/* DFD computation between two trajectories taken as a whole*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#define int long long
using namespace std;
int n, m;
vector<pair<int, int> > S1, S2;
vector<vector<int> > dfd_memo;

int distance(int i, int j){
    int d = (S1[i].first - S2[j].first)*(S1[i].first - S2[j].first);
    d = d + (S1[i].second-S2[j].second)*(S1[i].second-S2[j].second);
    return d;
}

int dfd(int i, int j){
    int ans = INT64_MAX;

    if(dfd_memo[i][j]!=-1){
        return dfd_memo[i][j];
    }

    if(i==0 && j==0){
        return dfd_memo[i][j] = distance(i, j);
    }
    
    if(i>0){
        ans = min(ans, dfd(i-1, j));
    }
    
    if(j>0){
        ans = min(ans, dfd(i, j-1));
    }

    if(i>0 && j>0){
        ans = min(ans, dfd(i-1, j-1));
    }

    ans = max(ans, distance(i, j));
    return ans;
}




int32_t main(){
    cin >> n >> m;
    S1.resize(n);
    S2.resize(m);
    dfd_memo.resize(n, vector<int>(m, -1));
    for(int i=0; i<n; i++){
        cin >> S1[i].first >> S1[i].second;
    }
    for(int i=0; i<m; i++){
        cin >> S2[i].first >> S2[i].second;
    }

    int r = dfd(n-1, m-1);
}
