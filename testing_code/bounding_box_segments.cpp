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

struct boundingBox{
    int start;
    int end;
    pair<double, double> width;
};

vector<vector<double> > line_segments; 

vector<pair<double, double> > points;
double abs_put_value(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{
    double put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(end.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -put_val;
    }
    return put_val;
}

double dist_between_points(pair<double, double> start, pair<double, double> end)
{
    double ans = sqrt((start.first - end.first)*(start.first - end.first) + (start.second - end.second)*(start.second - end.second));
    return ans;
}

double dist_from_line(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{

    double ans = abs_put_value(point, start, end)/dist_between_points(start, end);
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
}


vector<boundingBox> bounding_box_segmentations(vector<pair<double, double> > &points,int n, double max_percentage = 110.0)
{
    if(n>(int)points.size()){
        n = points.size();
    }
    int start = 0;
    double percentage;
    max_percentage -= 110.0;
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
        cout << "Starting new BB at: " << start << '\n';
        while(end<n){
            percentage = 110.0 + difference;
            pair<double, double> width = find_bounding_box(points, start, end);
            double current_ratio = (width.first + width.second) / dist_between_points(points[start], points[end]);
            double percentage_change = current_ratio*100 / minimum_prev_ratio;
            double width_change = (width.first + width.second) * 100 / (prev_width.first + prev_width.second);
            // cout << current_ratio << " " << prev_width << '\n';
            cout << end << "(" << points[end].first << ", " << points[end].second << ") " << percentage_change << " " << width_change << " " << percentage << '\n';
            if(end==n-1){
                end++;
                prev_width = width;
            }
            if((percentage<percentage_change && percentage < width_change )|| end==n){
                cout << "Ending the BB at: " << end << "(" << points[end].first << ", " << points[end].second << ")\n "<< '\n';
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

int find_dfd(int startFirst, int endFirst, int startSecond, int endSecond, vector<pair<double, double> >  &points){
    int n = points.size();
    vector<vector<double> > dfd(n, vector<double>(n, DBL_MAX));
    dfd[startFirst][startSecond] = dist_between_points(points[startFirst], points[startSecond]);
    for(int i = startSecond; i<endSecond+1; i++){
        double tmp = dfd[startFirst][i];
        dfd[startFirst][i+1] = max(tmp, dist_between_points(points[startFirst], points[i+1]));
    }

    for(int i=startFirst; i<endFirst; i++){
        double tmp = dfd[i][startSecond];
        dfd[i+1][startSecond] = max(tmp, dist_between_points(points[i+1], points[startSecond]));
    }

    for(int i=startFirst+1; i<endFirst+1; i++){
        for(int j=startSecond+1; j<endSecond+1; j++){
            dfd[i][j] = dist_between_points(points[i], points[j]);
            double tmp = min(dfd[i-1][j-1], dfd[i-1][j]);
            tmp = min(tmp, dfd[i][j-1]);
            dfd[i][j] = max(tmp, dfd[i][j]);

        }
    }

    return dfd[endFirst][endSecond];
}

void finding_motif(vector<pair<double, double> > &points, int k){
    vector<boundingBox> segments = bounding_box_segmentations(points, points.size(), 250);


}


void points_generator()
{
    double slope1 = 4.3, slope2 = -0.8;
    double intercept1 = 34.6, intercept2 = 9.1;
    srand(time(0));
    double last_x, last_y;
    for(int i=0;i<100;i++)
    {
        double x = i;
        double y = slope1*x + intercept1;
        last_x=x;
        last_y=y;
        pair<double, double> point(x,y);
        points.push_back(point);
    }
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
    return points;
}
int main()
{
    vector<pair<double, double> > points = read_dataset("/Users/ankushgarg/Desktop/DIP-Project/test_data/test_data7.csv");
    for(auto i: points){
        cout << i.first << " " << i.second << '\n';
    }
    fstream 
    // int n = 20;
    // vector<boundingBox> res = bounding_box_segmentations(points, 60, 250);
    // for(auto i: res){
    //     cout << "Starts at: " << i.start << '\n';
    //     cout << "Ends at:   " << i.end << '\n';
    //     cout << "Width is: " << i.width.first + i.width.second << '\n';
    //     cout << "Length is: " << dist_between_points(points[i.start], points[i.end]) << "\n\n";
    // }
    
}