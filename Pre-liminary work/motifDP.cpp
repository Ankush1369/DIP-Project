#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#define int long long
using namespace std;

int n, k; //k being then minimim motif length
vector<pair<int, int> > S; //input Trajectory
vector<vector<int> > dfd;

bool valid(int a, int b){
    return abs(a-b+1)>=k;
}

//Assuming cartersian coordinates for now
//Can be easily modified for any other form of coordinates
int distance(int i, int j){ 
    int d = (S[i].first - S[j].first)*(S[i].first - S[j].first);
    d = d + (S[i].second-S[j].second)*(S[i].second-S[j].second);
    return d;
}

vector<pair<int, int> > motif(){
    int bestDFD = INT64_MAX;
    vector<pair<int, int> > v(2);
    for(int startFirst = 0; startFirst < n && (startFirst+2*k-1) < n; startFirst++){
        for(int startSecond = startFirst + k; startSecond < n; startSecond++){
            dfd[startFirst][startSecond] = distance(startFirst, startSecond);
            
            for(int tmp = startSecond+1; tmp<n; tmp++){
                dfd[startFirst][tmp] = max(dfd[startFirst][tmp-1], distance(startFirst, tmp));
            } //initializing the DFD values when first trajectory acts as point trajectory

            for(int tmp = startFirst+1; tmp<startSecond; tmp++){
                dfd[tmp][startSecond] = max(dfd[tmp-1][startSecond], distance(tmp, startSecond));
            } //initializing the DFD values when second trajectory acts as point trajectory

            for(int endFirst = startFirst+1; endFirst < startSecond; endFirst++){
                for(int endSecond = startSecond+1; endSecond < n; endSecond++){
                    dfd[endFirst][endSecond] = distance(endFirst, endSecond);
                    int tmp = min(dfd[endFirst-1][endSecond], dfd[endFirst-1][endSecond-1]);
                    tmp = min(tmp, dfd[endFirst-1][endSecond-1]); 
                    //using the recursive relation for DFD computation
                    dfd[endFirst][endSecond] = max(dfd[endFirst][endSecond], tmp);
                    tmp = dfd[endFirst][endSecond];
                    if(valid(startFirst, endFirst) && valid(startSecond, endSecond) && tmp<bestDFD){
                        v[0] = {startFirst, endFirst};
                        v[1] = {startSecond, endSecond};
                        bestDFD = tmp;
                    } //updating the best motif if we found a valid new best pair
                }
            }

        }
    }

    return v;
    
}

int32_t main(){
    cin >> n >> k;
    S.resize(n);
    dfd.resize(n, vector<int>(n, INT64_MAX)); //initialzing dfd as max
    for(int i=0; i<n; i++){
        cin >> S[i].first >> S[i].second;
    }
    
}