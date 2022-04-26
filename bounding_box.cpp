#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<float.h>
#include<bits/stdc++.h>
using namespace std;

vector<vector<double>> line_segments; 

vector<pair<double, double>> points;
double abs_put_value(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{
    double put_val= round(10000*point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(end.second - start.second) - start.second*(end.first - start.first))/10000;
    if(put_val < 0)
    {
        return -put_val;
    }
    return put_val;
}

double dist_between_points(pair<double, double> start, pair<double, double> end)
{
    double ans = round(10000*sqrt((start.first - end.first)*(start.first - end.first) + (start.second - end.second)*(start.second - end.second)))/10000;
    return ans;
}

double dist_from_line(pair<double,double> point, pair<double, double> start, pair<double, double> end)
{
    double ans = round(10000*abs_put_value(point, start, end)/dist_between_points(start, end))/10000;
    return ans;
}

int put_bin(pair<double, double> point, pair< double, double> start, pair<double, double> end)
{
    double put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(end.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -1;
    }
    return 1;   
}

int bounding_box_segmentations(vector<pair<double, double>> points)
{
    int n = points.size();
    int start = 0;
    int end = 2;
    int curr_length = 0;
    while(end < n)
    {
        double min_bbox_positive = DBL_MAX;
        double min_bbox_negative = DBL_MAX;
        double length_min_bbox = 0;
        int breaking_point = 0;
        pair<double,double> min_bbox_posicoord(DBL_MAX, DBL_MAX);
        pair<double,double> min_bbox_negacoord(DBL_MAX, DBL_MAX);
        for(int i=end;i<n;i++)                                                                           
        {
            double max_dist_positive = 0;
            double max_dist_negative = 0;
            double length = dist_between_points(points[start], points[i]);
            pair<double, double> positive_coord(DBL_MAX, DBL_MAX);
            pair<double, double> negative_coord(DBL_MAX, DBL_MAX);
            for(int j=start+1;j<end;j++)
            {
                //double w = distance of points[j] from line formed between p[start] and p[i]
                double w = dist_from_line(points[j], points[start], points[i]);
                cout << w << '\n';
                if(put_bin(points[j], points[start], points[i]) >= 0 and w>max_dist_positive)
                {
                    max_dist_positive = w;
                    positive_coord = points[j];
                }
                if(put_bin(points[j], points[start], points[i]) < 0 and w>max_dist_negative)
                {
                    max_dist_negative = w;
                    negative_coord = points[j];
                }
            }
            if(min_bbox_positive + min_bbox_negative > (max_dist_positive+max_dist_negative)/length)
            {
                min_bbox_positive = max_dist_positive/length;
                min_bbox_negative = max_dist_negative/length;
                breaking_point = i;
                min_bbox_posicoord = positive_coord;
                min_bbox_negacoord = negative_coord;
                length_min_bbox = length;
            }
        }
        //line_segments.add((min_bbox_positive, min_bbox_negative, length_min_bbox, breaking_point, min_bbox_positivecoord, min_bbox_negativecoord))
        vector<double> line_seg;
        line_segments.push_back(line_seg);
        line_segments[curr_length].push_back(min_bbox_positive);
        line_segments[curr_length].push_back(min_bbox_negative);
        line_segments[curr_length].push_back(length_min_bbox);
        line_segments[curr_length].push_back(breaking_point);
        line_segments[curr_length].push_back(min_bbox_posicoord.first);
        line_segments[curr_length].push_back(min_bbox_posicoord.second);
        line_segments[curr_length].push_back(min_bbox_negacoord.first);
        line_segments[curr_length].push_back(min_bbox_negacoord.second);
        curr_length++;
        start = breaking_point + 1;
        end = start + 2;
    }
    if(end == n)
    {
        vector<double> line_seg;
        line_segments.push_back(line_seg);
        line_segments[curr_length].push_back(DBL_MAX);
        line_segments[curr_length].push_back(DBL_MAX);
        line_segments[curr_length].push_back(dist_between_points(points[n-2], points[n-1]));
        line_segments[curr_length].push_back(n-1);
        pair<double,double> min_bbox_posicoord(DBL_MAX, DBL_MAX);
        pair<double,double> min_bbox_negacoord(DBL_MAX, DBL_MAX);
        line_segments[curr_length].push_back(min_bbox_posicoord.first);
        line_segments[curr_length].push_back(min_bbox_posicoord.second);
        line_segments[curr_length].push_back(min_bbox_negacoord.first);
        line_segments[curr_length].push_back(min_bbox_negacoord.second);
        curr_length++;
        end=n;
    }
    // if only one point is left behind, we are ignoring it
    return curr_length;
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

int main()
{
    points_generator();
    // for(int i=0;i<100;i++)
    // {
    //     cout<<points[i].first<<' '<<points[i].second<<'\n';
    // }
    int segments = bounding_box_segmentations(points);
    cout << segments << '\n';

    int segs = line_segments.size();
    cout << segs << '\n';

    for(int i=0;i<segs;i++)
    {
        for(int j=0;j<line_segments[i].size();j++)
        {
            cout << line_segments[i][j] << ' ';
        }
        cout << '\n';
    }
}