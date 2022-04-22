#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
using namespace std;

vector<vector<float>> line_segments; 


float abs_put_value(pair<float,float> point, pair<float, float> start, pair<float, float> end)
{
    float put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(nd.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -put_val;
    }
    return put_val;
}

float dist_between_points(pair<float, float> start, pair<float, float> end)
{
    return sqrt((start.first - end.first)*(start.first - end.first) + (start.second - end.second)*(start.second - end.second))
}

float dist_from_line(pair<float,float> point, pair<float, float> start, pair<float, float> end)
{
    return abs_put_value(point, start, end)/dist_between_points(start, end);
}

int put_bin(pair<float, float> point, pair< float, float> start, pair<float, float> end)
{
    float put_val= point.second*(end.first - start.first) - point.first*(end.second - start.second) + start.first*(nd.second - start.second) - start.second*(end.first - start.first);
    if(put_val < 0)
    {
        return -1;
    }
    return 1;   
}

int bounding_box_segmentations(vector<pair<float, float>> points)
{
    int n = points.size();
    int start = 0;
    int end = 2;
    int curr_length = 0;
    while(end < n)
    {
        float min_bbox_positive = FLT_MAX;
        float min_bbox_negative = FLT_MAX;
        float length_min_bbox = 0;
        int breaking_point = 0;
        pair<float,float> min_bbox_posicoord(FLT_MAX, FLT_MAX);
        pair<float,float> min_bbox_negacoord(FLT_MAX, FLT_MAX);
        for(int i=end;i<n;i++)                                                                           
        {
            float max_dist_positive = 0
            float max_dist_negative = 0
            float length = dist(points[start], points[i])
            pair<float, float> positive_coord(FLT_MAX, FLT_MAX);
            pair<float, float> negative_coord(FLT_MAX, FLT_MAX);
            for(int j=start+1;j<end;j++)
            {
                //float w = distance of points[j] from line formed between p[start] and p[i]
                float w = dist_from_line(points[j], points[start], points[i]);
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
        vector<float> line_seg;
        line_segements.push_back(line_seg);
        line_segments[curr_length].push_back(min_bbox_positive);
        line_segments[curr_length].push_back(min_bbox_negative);
        line_segments[curr_length].push_back(length_min_bbox);
        line_segments[curr_length].push_back(breaking_point);
        line_segments[curr_length].push_back(min_bbox_positivecoord);
        line_segments[curr_length].push_back(min_bbox_positivecoord);
        curr_length++;
        start = breaking_point + 1;
        end = start + 2;
    }
    if(end == n)
    {
        vector<float> line_seg;
        line_segments.push_back(line_seg);
        line_segments[curr_length].push_back(FLT_MAX);
        line_segments[curr_length].push_back(FLT_MAX);
        line_segments[curr_length].push_back(dist_between_points(points[n-2], points[n-1]));
        line_segments[curr_length].push_back(n-1);
        pair<float,float> min_bbox_posicoord(FLT_MAX, FLT_MAX);
        pair<float,float> min_bbox_negacoord(FLT_MAX, FLT_MAX);
        line_segments[curr_length].push_back(min_bbox_posicoord);
        line_segments[curr_length].push_back(min_bbox_negacoord);
        curr_length++;
        end=n;
    }
    // if only one point is left behind, we are ignoring it
    return curr_length;
}
