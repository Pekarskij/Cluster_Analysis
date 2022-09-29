#pragma once
#include "Find_cluster.h"
class ha
{
private:
    int k;
    int t;
public:
    double distance_for_hierarchy(vector <int>& a, vector <int>& b, vector <Point>& points);
   // Find_cluster find_clusters(vector <Point>& points);
    Find_cluster hierarchy(Field *field);
    vector <int> tree_walk(vector <int>& T, int n, vector <vector <int>>& point_indicators);
    void print_tree(ofstream& file, vector <Point>& points, int n, vector <vector <int>>& point_indicators);
    ha();

    ha(const ha& h);

    const ha& operator=(const ha& h);

    void assing_k(int k_k);
    void assing_t(int t_t);
};

