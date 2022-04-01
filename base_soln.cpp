#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#define int long long
using namespace std;

int n;
vector<pair<float, float> > S; // input Trajectory
vector<vector<float> > dfd;
float R = 6371.00;

bool valid(int a, int b, int k)
{
    return abs(b - a + 1) >= k;
}

// Assuming grid coordinates as in dataset
// Can be easily modified for any other form of coordinates
float spherical_distance(int i, int j)
{
    float d = 2 * R * asin(sqrt(sin((S[j].first - S[i].first) / 2) * sin((S[j].first - S[i].first) / 2) + cos(S[i].first) * cos(S[j].first) * sin((S[j].second - S[i].second) / 2) * sin((S[j].second - S[i].second) / 2)));
    return d;
}

float euclidean_distance(int i, int j)
{
    float d = sqrt(pow(S[i].first - S[j].first, 2) + pow(S[i].second - S[j].second, 2));
    return d;
}

float distance(int i, int j)
{
    return spherical_distance(i, j);
}



int read_dataset(string fname)
{
    string line, word;
    int k = 0;
    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (k == 1)
            {
                // row.clear();
                int i = 0;
                pair<float, float> row;
                stringstream str(line);

                while (getline(str, word, ','))
                {
                    if (i == 0)
                    {
                        row.first = stof(word);
                        i++;
                    }
                    if (i == 1)
                    {
                        row.second = stof(word);
                        i++;
                    }
                }
                S.push_back(row);
            }
            k = 1;
        }
    }
    else
        cout << "Could not open the file\n";
    return S.size();
}

vector<pair<int, int> > motif(int k)
{
    float bestDFD = 100000000;
    vector<pair<int, int> > v(2);
   // cout << n << '\n';
    for (int startFirst = 0; startFirst < n && (startFirst + 2 * k - 1) < n; startFirst++)
    {   
        for (int startSecond = startFirst + k; startSecond < n; startSecond++)
        {
            dfd[startFirst][startSecond] = distance(startFirst, startSecond);
            if(dfd[startFirst][startSecond]>bestDFD){
                continue;
                //loose lower bound on DFD which is the distance between starting points
            }
            


            for (int tmp = startSecond + 1; tmp < n; tmp++)
            {
                dfd[startFirst][tmp] = max(dfd[startFirst][tmp - 1], distance(startFirst, tmp));
            } // initializing the DFD values when first trajectory acts as point trajectory

            for (int tmp = startFirst + 1; tmp < startSecond; tmp++)
            {
                dfd[tmp][startSecond] = max(dfd[tmp - 1][startSecond], distance(tmp, startSecond));
            } // initializing the DFD values when second trajectory acts as point trajectory

            for (int endFirst = startFirst + 1; endFirst < startSecond; endFirst++)
            {
                for (int endSecond = startSecond + 1; endSecond < n; endSecond++)
                {
                    dfd[endFirst][endSecond] = distance(endFirst, endSecond);
                    float tmp = min(dfd[endFirst - 1][endSecond], dfd[endFirst - 1][endSecond - 1]);
                    tmp = min(tmp, dfd[endFirst - 1][endSecond - 1]);
                    // using the recursive relation for DFD computation
                    dfd[endFirst][endSecond] = max(dfd[endFirst][endSecond], tmp);
                    tmp = dfd[endFirst][endSecond];
                    if (valid(startFirst, endFirst, k) && valid(startSecond, endSecond, k) && tmp < bestDFD)
                    {
                        v[0] = {startFirst, endFirst};
                        v[1] = {startSecond, endSecond};
                        bestDFD = tmp;
                    } // updating the best motif if we found a valid new best pair
                }
            }
        }
    }

    return v;
}

void plt_to_csv(string filename, string outfile)
{
    string line;
    ifstream b_file(filename);
    ofstream output_file;
    output_file.open(outfile);
    int i = 0;
    while (getline(b_file, line))
    {
        if (i > 6)
        {
            output_file << line << endl;
        }
        i++;
    }
}

int32_t main()
{
    string filename = "dataset.csv";
    // vector<float> xc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    // vector<float> yc = {5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, -5, -4, -3, -2, -1, 0, 0, 0, 0, 0, 0, 0};
    //sample testcase 
    cout << "Enter plt filename for Geolife dataset: ";
    string pltname;
    cin >> pltname;
    plt_to_csv(pltname, filename);
    n = read_dataset(filename);

    dfd.resize(n, vector<float>(n, 100000000)); // initialzing dfd as max
    int m, motif_length;
    cout << "Enter tarjectory length to be analyized: ";
    cin >> m;
    cout << "Enter motif_length to be considered: ";
    cin >> motif_length;
    n = min(n, m);
    motif_length = min(n/2, motif_length);
    vector<pair<int, int> > res = motif(motif_length);
    for(auto i: res){
        cout << i.first << " "  << i.second << '\n';
    }
}