#pragma once
#include "Find_cluster.h"

class Spanning_tree
{
public:
    vector <pair <double, pair <int, int>>> creat_spanning_tree(vector <Point>& points);
    void print_spanning_tree(vector <Point>& points, vector <pair <double, pair <int, int>>>& edges);
};

