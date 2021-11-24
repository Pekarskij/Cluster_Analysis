#pragma once
#include "Find_cluster.h"

class ha
{
private:
    int k;
    int t;
public:
    double distance_for_hierarchy(vector <Point> points, vector <int> a, vector <int> b, int t);
    Find_cluster find_clusters(vector <Point>& points);
    ha();

    ha(const ha& h);

    const ha& operator=(const ha& h);

    void assing_k(int k_k);
    void assing_t(int t_t);
};