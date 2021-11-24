#include "ha.h"
ha::ha() { k = 0; t = 0; }

ha::ha(const ha& h) { k = h.k; t = h.t; }

const ha& ha::operator=(const ha& h) { k = h.k; t = h.t; return *this; }

void ha::assing_k(int k_k) { k = k_k; }

void ha::assing_t(int t_t) { t = t_t; }

double ha::distance_for_hierarchy(vector <Point> points, vector <int> a, vector <int> b, int t)
{
    int n = a.size(), m = b.size(), i, j;
    double dist = 0;
    double temp;
    if (t == 1)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++) dist = dist + (points[a[i]] - points[b[j]]).length();
        }
        dist = dist / (n * m);
    }
    if (t == 2)
    {
        dist = (points[a[0]] - points[b[0]]).length();
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                temp = (points[a[i]] - points[b[j]]).length();
                   if (temp < dist)
                   {
                       dist = temp;
                   }
            }
        }
    }
    if (t == 3)
    {
        dist = (points[a[0]] - points[b[0]]).length();
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                temp = (points[a[i]] - points[b[j]]).length();
                    if (temp > dist)
                    {
                        dist = temp;
                    }
            }
        }
    }
    return dist;
}

Find_cluster ha::find_clusters(vector <Point>& points)
{
    int  n = points.size();
    vector <vector <int>> point_indicators(n);
    vector <Cluster> P(k);
    Find_cluster fc;
    int min_j = 0, min_c = 0;
    double min, temp;
    for (int i = 0; i < n; i++)
    {
        point_indicators[i].push_back(i);
        for (int j = 0; j < k; j++) P[j].add_point_indicator(0);
    }
    for (int i = 0; i < n - k; i++)
    {
        min = distance_for_hierarchy(points, point_indicators[0], point_indicators[1], t);
         min_j = 0, min_c = 0;
        for (int j = 0; j < n - i; j++)
        {
            for (int c = 0; c < j; c++)
            {
                 temp = distance_for_hierarchy(points, point_indicators[j], point_indicators[c], t);
                if (temp < min)
                {
                    min = temp;
                    min_j = j;
                    min_c = c;
                }
            }
        }
        for (int t = 0; t < point_indicators[min_j].size(); t++)
        {
            point_indicators[min_c].push_back(point_indicators[min_j][t]);
        }
        point_indicators.erase(point_indicators.begin() + min_j);
    }
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < point_indicators[i].size(); j++)
        {
            P[i].assign_point_indicator(point_indicators[i][j], 1);
        }
    }
    for (int i = 0; i < k; i++)
    {
        fc.add_cluster(P[i]);
    }
    point_indicators.clear();
    return fc;
}