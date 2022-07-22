#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <float.h>
#include <cmath>
using namespace std;

// int n;
// int test_file_number;
// int slicing_start = 0, slicing_end = -1;
// vector<pair<double, double> > S; // input Trajectory
// vector<vector<double> > dfd;
double R = 6371.00;

bool valid(int a, int b, int k)
{
    return abs(b - a + 1) >= k;
}

// Assuming grid coordinates as in dataset
// Can be easily modified for any other form of coordinates
// double spherical_distance(int i, int j)
// {
//     double d = 2 * R * asin(sqrt(sin((S[j].first - S[i].first) / 2) * sin((S[j].first - S[i].first) / 2) + cos(S[i].first) * cos(S[j].first) * sin((S[j].second - S[i].second) / 2) * sin((S[j].second - S[i].second) / 2)));
//     return d;
// }

double euclidean_distance(vector<pair<double, double> > &S, int i, int j)
{
    double d = sqrt(pow(S[i].first - S[j].first, 2) + pow(S[i].second - S[j].second, 2));
    return d;
}

double distance(vector<pair<double, double> > &S, int i, int j)
{
    return euclidean_distance(S, i, j);
}

vector<pair<double, double> > read_dataset(string fname)
{
    vector<pair<double, double> >  points;
    string line, word;
    int k = 0;
    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if(k==0){
                k++;
                continue;
            }
            int i = 0;
            pair<double, double> row;
            stringstream str(line);
            getline(str, word, ',');
            row.first = stof(word);
            getline(str, word);
            row.second = stof(word);
            points.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";
    file.close();
    return points;
}

// void printDFD(int i, int j){
//     cout << "";
//     // cout << "DFD for " << i << " and " << j << "is :" << dfd[i][j] << '\n';
// }

void printConsole(vector<pair<int, int> > motifPair, int slice = 0){
    cout << "The motif from recursive DP solution is as follows:\n";
    
    cout << "First subtrajectory is as follows:\n";
    cout << motifPair[0].first + slice << " " << motifPair[0].second + slice << '\n';

    cout << "Second subtrajectory is as follows:\n";
    cout << motifPair[1].first + slice << " " << motifPair[1].second + slice << '\n';

}

// void printToFile(vector<pair<int, int> > motifPair, int slice = 0){
//     string motif_path = "./test_data" + to_string(test_file_number) + "/DP_motif.txt";
//     fstream motif_output(motif_path, ios::out);
   
//     // motif_output << "First subtrajectory is as follows:\n";
//     motif_output << motifPair[0].first + slice << endl << motifPair[0].second + slice << endl;

//     // motif_output << "Second subtrajectory is as follows:\n";
//     motif_output << motifPair[1].first + slice << endl << motifPair[1].second + slice << endl;
// }

vector<pair<double, double> >  slice(vector<pair<double, double> > points, int start = 0, int end = 0){
    vector<pair<double, double> > S;
    int n = points.size();
    if(end==0){
        end = points.size();
    }else{
        end = (end % n + n) % n;
    }

    for(int i = start; i<end; i++){
        S.push_back(points[i]);
    }

    return S;
}




void find_motif(vector<pair<double, double> > S, int k, int slice_start = 0)
{   
    double bestDFD = __DBL_MAX__;
    vector<pair<int, int> > v(2);
    int n = S.size();
    vector<vector<double> > dfd(n+1, vector<double>(n+1, -1));
    for (int startFirst = 0; startFirst < n && (startFirst + 2 * k - 1) < n; startFirst++)
    {   
        for (int startSecond = startFirst + k; startSecond < n; startSecond++)
        {
            dfd[startFirst][startSecond] = distance(S, startFirst, startSecond);
            if(dfd[startFirst][startSecond]>bestDFD){
                continue;
                //loose lower bound on DFD which is the distance between starting points
            }
            

            for (int tmp = startFirst + 1; tmp < n; tmp++)
            {
                dfd[tmp][startSecond] = max(dfd[tmp - 1][startSecond], distance(S, tmp, startSecond));
                dfd[startFirst][tmp] = max(dfd[startFirst][tmp - 1], distance(S, startFirst, tmp));
            } // initializing the DFD values when second or first trajectory acts as point trajectory

            for (int endFirst = startFirst + 1; endFirst < startSecond; endFirst++)
            {
                for (int endSecond = startSecond + 1; endSecond < n; endSecond++)
                {
                    // using the recursive relation for DFD computation 
                    dfd[endFirst][endSecond] = distance(S, endFirst, endSecond);
                    double tmp = min(dfd[endFirst - 1][endSecond], dfd[endFirst - 1][endSecond - 1]);
                    tmp = min(tmp, dfd[endFirst][endSecond - 1]);
                    dfd[endFirst][endSecond] = max(dfd[endFirst][endSecond], tmp);
                    
                    // printDFD(endFirst, endSecond);
                    
                    tmp = dfd[endFirst][endSecond];
                    if (valid(startFirst, endFirst, k) && valid(startSecond, endSecond, k) && (tmp < bestDFD || bestDFD<0))
                    {
                        v[0] = {startFirst, endFirst};
                        v[1] = {startSecond, endSecond};
                        bestDFD = tmp;
                    } // updating the best motif if we found a valid new best pair
                }
            }
        }
    }

    printf("Best DFD is: %f\nStarting of First Trajectory: %d\nEnd of First Trajectory: %d\nStart of Second Trajectory: %d\nEnd of Second Trajectory: %d\n", bestDFD, v[0].first + slice_start, v[0].second + slice_start, v[1].first + slice_start, v[1].second + slice_start);
}


void findMotifDP(string filepath, int motif_length = 20,  int start_index = 0, int ending_index = 0){
    vector<pair<double, double> > dataSet = read_dataset(filepath);
    vector<pair<double, double> > S = slice(dataSet, start_index, ending_index);
    if(motif_length<0){
        motif_length = 20;
    }
    find_motif(S, motif_length, start_index);
}



int32_t main()
{
    int start_index = 100;
    int ending_index = 300;
    int motif_length = 15;
    string filepath = "./data2.csv";
    findMotifDP(filepath, motif_length, start_index, ending_index);

}