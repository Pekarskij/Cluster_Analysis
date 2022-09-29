#pragma once
#include "Find_cluster.h"
class Spanning_tree
{
private:
    double delta;
public:
    Spanning_tree();
    ~Spanning_tree() = default;
    double get_delta();
    vector <pair <double, pair <int, int>>> creat_spanning_tree(Field *field);
    void print_spanning_tree(Field *field, vector <pair <double, pair <int, int>>>& edges);
};

