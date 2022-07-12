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

int n;
int test_file_number;
int slicing_start = 0, slicing_end = -1;
vector<pair<double, double> > S; // input Trajectory
vector<vector<double> > dfd;
double R = 6371.00;

bool valid(int a, int b, int k)
{
    return abs(b - a + 1) >= k;
}

// Assuming grid coordinates as in dataset
// Can be easily modified for any other form of coordinates
double spherical_distance(int i, int j)
{
    double d = 2 * R * asin(sqrt(sin((S[j].first - S[i].first) / 2) * sin((S[j].first - S[i].first) / 2) + cos(S[i].first) * cos(S[j].first) * sin((S[j].second - S[i].second) / 2) * sin((S[j].second - S[i].second) / 2)));
    return d;
}

double euclidean_distance(int i, int j)
{
    double d = sqrt(pow(S[i].first - S[j].first, 2) + pow(S[i].second - S[j].second, 2));
    return d;
}

double distance(int i, int j)
{
    return euclidean_distance(i, j);
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

void printDFD(int i, int j){
    cout << "";
    // cout << "DFD for " << i << " and " << j << "is :" << dfd[i][j] << '\n';
}

void printConsole(vector<pair<int, int> > motifPair, int slice = 0){
    cout << "The motif from recursive DP solution is as follows:\n";
    
    cout << "First subtrajectory is as follows:\n";
    cout << motifPair[0].first + slice << " " << motifPair[0].second + slice << '\n';

    cout << "Second subtrajectory is as follows:\n";
    cout << motifPair[1].first + slice << " " << motifPair[1].second + slice << '\n';

}

void printToFile(vector<pair<int, int> > motifPair, int slice = 0){
    string motif_path = "./test_data" + to_string(test_file_number) + "/DP_motif.txt";
    fstream motif_output(motif_path, ios::out);
   
    // motif_output << "First subtrajectory is as follows:\n";
    motif_output << motifPair[0].first + slice << endl << motifPair[0].second + slice << endl;

    // motif_output << "Second subtrajectory is as follows:\n";
    motif_output << motifPair[1].first + slice << endl << motifPair[1].second + slice << endl;
}

void points_slice(vector<pair<double, double> > points, int start = 0, int end = -1){
    S.clear();
    int n = points.size();
    if(end==-1){
        end = points.size();
    }else{
        end = (end % n + n) % n;
    }

    for(int i = start; i<end; i++){
        S.push_back(points[i]);
    }

    dfd.clear();
    n = S.size();

    dfd.resize(n+1, vector<double>(n+1, -1));
}




void find_motif(int k)
{
    double bestDFD = __DBL_MAX__;
    vector<pair<int, int> > v(2);
    int n = S.size();
   // cout << n << '\n';
    for (int startFirst = 0; startFirst < n && (startFirst + 2 * k - 1) < n; startFirst++)
    {   
        // cout << "Starting first trajectory at " << startFirst << '\n';
        for (int startSecond = startFirst + k; startSecond < n; startSecond++)
        {
            // cout << "\t Start second trajectory at " << startSecond << '\n';
            dfd[startFirst][startSecond] = distance(startFirst, startSecond);
            printDFD(startFirst, startSecond);
            if(dfd[startFirst][startSecond]>bestDFD){
                continue;
                //loose lower bound on DFD which is the distance between starting points
            }
            

            for (int tmp = startFirst + 1; tmp < n; tmp++)
            {
                dfd[tmp][startSecond] = max(dfd[tmp - 1][startSecond], distance(tmp, startSecond));
                dfd[startFirst][tmp] = max(dfd[startFirst][tmp - 1], distance(startFirst, tmp));
                printDFD(startFirst, tmp);
                printDFD(tmp, startSecond);
            } // initializing the DFD values when second or first trajectory acts as point trajectory

            for (int endFirst = startFirst + 1; endFirst < startSecond; endFirst++)
            {
                for (int endSecond = startSecond + 1; endSecond < n; endSecond++)
                {
                    // using the recursive relation for DFD computation 
                    dfd[endFirst][endSecond] = distance(endFirst, endSecond);
                    double tmp = min(dfd[endFirst - 1][endSecond], dfd[endFirst - 1][endSecond - 1]);
                    tmp = min(tmp, dfd[endFirst][endSecond - 1]);
                    dfd[endFirst][endSecond] = max(dfd[endFirst][endSecond], tmp);
                    
                    printDFD(endFirst, endSecond);
                    
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
    cout << "The best DFD is " << bestDFD << '\n';
    printConsole(v, slicing_start);
    // printToFile(v, slicing_start);
    cout << '\n';
}



int32_t main(int argc, char* argv[])
{
    // if(argc!=2){
    //     cout << "Enter testing file number as a Command Line Argument \nPossible values are 7 and 9";
    //     return 0;
    // }
    // try {
    //     test_file_number = atoi(argv[1]);
    //     if(test_file_number!=7 && test_file_number!=9){
    //         throw 404;
    //     }else{
    //         cout << "Starting testing on respective file...\n";
    //     }
    // } catch (...){
    //     cout << "Invalid file number, it must be either 7 or 9";
    //     return 0;
    // }
    // string inputpath = "./test_data" + to_string(test_file_number) + "/test_data.csv";
    string inputpath = "./data_2.csv";
    vector<pair<double, double> > points = read_dataset(inputpath);

    cout << "Start slicing at: ";
    cin >> slicing_start;
    cout << "End slicing at: ";
    cin >> slicing_end;
    points_slice(points, slicing_start, slicing_end);
    int motif_length = 25;
    while(motif_length > 0){
        cout << "Enter motif_length: ";
        cin >> motif_length;
        find_motif(motif_length);
    }
}