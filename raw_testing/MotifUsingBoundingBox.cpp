#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <float.h>
#include <sstream>
#include <algorithm>
using namespace std;


int test_file_number;
struct boundingBox{
    int start;
    int end;
    pair<double, double> width;
};

struct triplet{
    int first, second;
    double lower_bound;
};

struct result_motif {
    int startFirst, startSecond, endFirst, endSecond;
    double dfd;
};

void print(result_motif x, int offset = 0){
    // cout << "This motif has dfd: " << x.dfd << '\n';
    // cout << "Starts first at: " << x.startFirst << "\nEnds first at: " << x.endFirst;
    // cout << "\nStart second at: " << x.startSecond << "\nEnds second at: " << x.endSecond << "\n\n";
    printf("DFD is: %f\nStarting of First Trajectory: %d\nEnd of First Trajectory: %d\nStart of Second Trajectory: %d\nEnd of Second Trajectory: %d\n\n", x.dfd, x.startFirst + offset, x.endFirst + offset, x.startSecond + offset, x.endSecond + offset);

}

bool cmp_motif(result_motif a, result_motif b){
    return a.dfd < b.dfd;
}

bool cmp(triplet a, triplet b){
    return a.lower_bound < b.lower_bound;
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

double abs_put_value(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{
    double put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(end.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -put_val;
    }
    return put_val;
}

double distance_between_points(pair<double, double> start, pair<double, double> end)
{
    double ans = sqrt((start.first - end.first)*(start.first - end.first) + (start.second - end.second)*(start.second - end.second));
    return ans;
}

double dist_from_line(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{

    double ans = abs_put_value(point, start, end)/distance_between_points(start, end);
    return ans;
} //distance of a point from line

int find_region(pair<double, double> point, pair< double, double> start, pair<double, double> end)
{
    double put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(end.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -1;
    }
    return 1;   
} //checking sign of point wrt to a a line joining two points

pair<double, double> find_bounding_box(vector<pair<double, double> > &points, int start, int end){
    double max_upper = 0;
    double max_lower = 0;
    for(int i=start+1; i<end; i++){
        if(find_region(points[i], points[start], points[end]) > 0){
            max_upper = max(max_upper, dist_from_line(points[i], points[start], points[end]));
        }else{
            max_lower = max(max_lower, dist_from_line(points[i], points[start], points[end]));
        }
    }
    pair<double, double> res(max_upper, max_lower);
    return res;
} //finding box given the end points of the box 


vector<boundingBox> bounding_box_segmentations(vector<pair<double, double> > &points,int n, double max_percentage = 105.0)
{
    if(n>(int)points.size()){
        n = points.size();
    }
    int start = 0;
    double percentage;
    max_percentage -= 105.0;
    if(max_percentage<0){
        max_percentage = 0;
    }
    vector<boundingBox> res;
    while(start<n)
    {
        int end = start+2;
        double difference = max_percentage;
        double percentage;
        double minimum_prev_ratio = DBL_MAX;
        pair<double, double> prev_width(DBL_MAX, DBL_MAX);
        bool found = false;
        // cout << "Starting new BB at: " << start << '\n';
        while(end<n){
            percentage = 105.0 + difference;
            pair<double, double> width = find_bounding_box(points, start, end);
            double current_ratio = (width.first + width.second) / distance_between_points(points[start], points[end]);
            double percentage_change = current_ratio*100 / minimum_prev_ratio;
            double width_change = (width.first + width.second) * 100 / (prev_width.first + prev_width.second);
            // cout << current_ratio <<  " "  ;
            // cout << prev_width.first + prev_width.second << '\n';
            // cout << end << "(" << points[end].first << ", " << points[end].second << ") " << percentage_change << " " << width_change << " " << percentage << '\n';
            if(end==n-1){
                end++;
                prev_width = width;
            }
            if((percentage<percentage_change && percentage < width_change )|| end==n){
                // cout << "Ending the BB at: " << end << "(" << points[end].first << ", " << points[end].second << ")\n "<< '\n';
                boundingBox latest;
                latest.start = start;
                latest.end = end-1;
                latest.width = prev_width;
                res.push_back(latest);
                start = end-1;
                difference = difference * 0.9;
                found = true;
                break;
            }
            prev_width = width;
            minimum_prev_ratio = current_ratio;
            end++;
            difference = difference*0.9;
        }
        if(end>=n){
            break;
        }

    }
    return res;
}

//function to compute discrte frechet distance between two subtrajectories of a main trajectory
double find_dfd(int startFirst, int endFirst, int startSecond, int endSecond, vector<pair<double, double> >  &points){
    int n = points.size();
    vector<vector<double> > dfd(n, vector<double>(n, DBL_MAX));
    dfd[startFirst][startSecond] = distance_between_points(points[startFirst], points[startSecond]);
    // cout << "dfd of starting points: " << dfd[startFirst][startSecond] << '\n';
    for(int i = startSecond; i<endSecond+1; i++){
        double tmp = dfd[startFirst][i];
        dfd[startFirst][i+1] = max(tmp, distance_between_points(points[startFirst], points[i+1]));
        // cout << startFirst << " and " << i+1 << " has dfd: "<< dfd[startFirst][i+1] << '\n';
    }

    for(int i=startFirst; i<endFirst; i++){
        double tmp = dfd[i][startSecond];
        dfd[i+1][startSecond] = max(tmp, distance_between_points(points[i+1], points[startSecond]));
        // cout << i+1 << " and " << startSecond << " has dfd: "<< dfd[i+1][startSecond] << '\n';
    }

    for(int i=startFirst+1; i<endFirst+1; i++){
        for(int j=startSecond+1; j<endSecond+1; j++){
            dfd[i][j] = distance_between_points(points[i], points[j]);
            // cout << i << " and " << j << " has dfd: " << dfd[i][j] << '\n';
            double tmp = min(dfd[i-1][j-1], dfd[i-1][j]);
            tmp = min(tmp, dfd[i][j-1]);
            dfd[i][j] = max(tmp, dfd[i][j]);

        }
    }

    return dfd[endFirst][endSecond];
}



void finding_motif(vector<pair<double, double> > &points, int k, int percentage_change = 300, int start_index = 0 ){
    vector<boundingBox> segments = bounding_box_segmentations(points, points.size(), percentage_change);
    string outpath = "./test_data" + to_string(test_file_number) + "/";
    string outpath_bounding = outpath + "BB_segments.csv";
    fstream output_bounding(outpath_bounding, ios::out);
    output_bounding << "start" << endl;
    //Storing bounding box segments in a file
    for(auto i: segments){
        output_bounding << i.start << endl;
    }

    int m = segments.size();
    vector<triplet> bb_pairs(m*(m-1)/2);
    int cnt = 0;
    //Sorting all bounding box pars based on lower bound on their dfd
    for(int i=0; i<m; i++){
        for(int j=i+1; j<m; j++){
            double d1 = distance_between_points(points[segments[i].start], points[segments[j].end]);
            double d2 = distance_between_points(points[segments[i].end], points[segments[j].end]);
            bb_pairs[cnt].first = i;
            bb_pairs[cnt].second = j;
            bb_pairs[cnt].lower_bound = max(d1, d2);
            cnt++;
        }
    }
    sort(bb_pairs.begin(), bb_pairs.end(), cmp);
    
    //Finding bounding box with minimum dfd
    vector<result_motif> results;
    double upper_bound = DBL_MAX;
    pair<int, int> best;
    bool flag = false;
    for(auto i: bb_pairs){

        boundingBox first_trajectory = segments[i.first];
        boundingBox second_trajectory = segments[i.second];

        int l1 = first_trajectory.end - first_trajectory.start+1;
        int l2 = second_trajectory.end - second_trajectory.start+1;
        l1 = min(l1, l2);
        if(l1<k){
            continue;
        }

        double tmp_dfd = find_dfd(first_trajectory.start, first_trajectory.end, second_trajectory.start, second_trajectory.end, points);
        result_motif tmp_motif;
        tmp_motif.startFirst = first_trajectory.start;
        tmp_motif.endFirst = first_trajectory.end;
        tmp_motif.startSecond = second_trajectory.start;
        tmp_motif.endSecond = second_trajectory.end;
        tmp_motif.dfd = tmp_dfd;

        results.push_back(tmp_motif);
        if(tmp_dfd<upper_bound){
            upper_bound = tmp_dfd;
            best.first = i.first;
            best.second = i.second;
            flag = true;
        }
    }

    sort(results.begin(), results.end(), cmp_motif);
    string postionArr[] = {"First", "Second", "Third", "Fourth", "Fifth"};
    // printf("%d, %d, ", k, percentage_change);
    for(int i=0; i<5 &&  i < (int)results.size(); i++){
        // printf("%d, %d, ", k, percentage_change);
        cout << postionArr[i] << " best trajectory is: \n";
        print(results[i], start_index);
    }

    printf("\n");


}

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

void findMotifUsingBox(string filepath, int motif_length = 20, int percentage = 300, int start_index = 0, int ending_index = 0){
    vector<pair<double, double> > dataSet = read_dataset(filepath);
    vector<pair<double, double> > S = slice(dataSet, start_index, ending_index);
    if(motif_length<0){
        motif_length = 20;
    }
    if(percentage < 110){
        percentage = 110;
    }
    finding_motif(S, motif_length, percentage, start_index);
}


int main()
{
    int start_index = 0;
    int ending_index = 0;
    int motif_length = 15;
    int percentage = 300;
    string filepath = "./data2.csv";
    findMotifUsingBox(filepath, motif_length, percentage, start_index, ending_index);
    
}



