#include <iostream>
#include <vector>
#include <algorithm>
#define double long double
using namespace std;
// int min(int a, int b)
// {
//     if (a > b)
//     {
//         return b;
//     }
//     return a;
// }
// int max(int a, int b)
// {
//     if (a < b)
//     {
//         return b;
//     }
//     return a;
// }
//-1 t be replaced by infinity
vector<vector<double> > E, F, A, B, X;
vector<vector<int> > I;

void reset(int n, int t)
{
    E.clear(); E.resize(n, vector<double>(n, 0.0));
    F.clear(); F.resize(n, vector<double>(t+1, 0.0));
    A.clear(); A.resize(n, vector<double>(n, 0.0));
    B.clear(); B.resize(n, vector<double>(n, 0.0));
    X.clear(); X.resize(n, vector<double>(t+1, 0.0));
    I.clear(); I.resize(n, vector<int>(t+1, 0));
}

bool compare(double a, double b)
{
    double c = abs(a-b);
    double max_error = 1e-6;
    return c>max_error;
}

vector<int> piecewise_continuous(vector<pair<double, double> > S, int T)
{
    int n = S.size();
    reset(n, T);
   // cout << "Resetting done\n";
    vector<double> product_xy(n + 1, 0.0);
    vector<double> sum_x(n + 1, 0.0);
    vector<double> sum_y(n + 1, 0.0);
    vector<double> sum_x2(n + 1, 0.0);
    double pxy = 0, sx = 0, sy = 0, sx2 = 0;
    for (int i = 0; i < n; i++)
    {
        pxy += S[i].first * S[i].second;
        product_xy[i + 1] = pxy;
        sx += S[i].first;
        sum_x[i + 1] = sx;
        sy += S[i].second;
        sum_y[i + 1] = sy;
        sx2 += S[i].first * S[i].first;
        sum_x2[i + 1] = sx2;
    }
    for (int i = 0; i < n; i++)
    {
        E[i][i] = INT32_MAX;
        for (int j = i + 1; j < n; j++)
        {

            double slope = (j - i + 1) * (product_xy[j + 1] - product_xy[i]) - (sum_x[j + 1] - sum_x[i]) * (sum_y[j + 1] - sum_y[i]);
            double tmp = (j - i + 1) * (sum_x2[j + 1] - sum_x2[i]) - (sum_x[j + 1] - sum_x[i]) * (sum_x[j + 1] - sum_x[i]);
            slope = slope / tmp;

            double intercept = ((sum_y[j + 1] - sum_y[i]) - slope * (sum_x[j + 1] - sum_x[i])) / (j - i + 1);
            A[i][j] = slope;
            B[i][j] = intercept;
            E[i][j] = 0;
            for (int k = i; k <= j; k++)
            {
                E[i][j] += (S[k].second - slope * S[k].first - intercept) * (S[k].second - slope*S[k].first - intercept);
            }
        }
    }
    
    for (int j = 0; j < n; j++)
    {
        for (int t = j+1; t <= T; t++)
        {
            F[j][t] = -1;
            I[j][t] = 0;
            X[j][t] = 0;
        }
        F[j][1] = E[0][j];
        I[j][1] = 1;
        X[j][1] = 0;
        for (int t = 2; t <= min(j-1, T); t++)
        {
            F[j][t] = INT32_MAX;
            I[j][t] = 0;
            X[j][1] = 0;
           // cout << j << " " << '\n';
            for (int i = t; i<j; i++)
            {
               // cout << "Finding index "  << i << " " << j << '\n';
                int k = I[i][t - 1];
                if (k != 0 && compare(A[k][i], A[i][j]))
                {
                    double x = (B[k][i] - B[i][j]) / (A[i][j] - A[k][i]);
                    if (x >= S[max(0, i)].first && x <= S[min(n - 1, i)].first && (F[j][t] > F[i][t - 1] + E[i][j]) )
                    {
                        F[j][t] = F[i][t - 1] + E[i][j];
                        I[j][t] = i;
                        X[j][t] = x;
                    }
                }
            }
        }
    }

    vector<int> res;
    int idx = n-1;
    int point = T;
    res.push_back(idx);
    while(point>0){
        res.push_back(I[idx][point]);
        idx = I[idx][point];
        point--;
    }
    reverse(res.begin(), res.end());
    return res;
}

int main()
{
    vector<double> xc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    vector<double> yc = {5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, -5, -4, -3, -2, -1, 0, 0, 0, 0, 0, 0, 0};
    vector<pair<double, double> > S;
    int n = 12;
    for(int i=0; i<n; i++){
        S.push_back({xc[i], yc[i]});
    }
    int t = 1;
    while(t<n){
        vector<int> ans;
        ans = piecewise_continuous(S, t);
        cout << "When t is " << t << ": ";
        for(auto i: ans){
            cout << i << " ";
        }
        cout << '\n';
        t++;
    }
    // int t = I[0].size();
    // cout << I[0].size() << '\n';
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<t+1; j++){
    //         cout << I[i][j] << " " ;
    //     }
    //     cout << '\n';
    // }

    return 0;
}