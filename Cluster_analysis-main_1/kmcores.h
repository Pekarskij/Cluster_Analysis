#pragma once
#include "Find_cluster.h"
#include "Field.h"
class kmcores
{
private:
    int k, p;
public:
    kmcores();
    kmcores(const kmcores& km);
    const kmcores& operator=(const kmcores& km);
    ~kmcores() = default;
    void assing_k(int k_k);
    void assing_p(int p_p);
    Find_cluster find_clusters(Field *field, vector<vector<Point>>& kmcores);
    void print_iter(vector <Cluster> P, Field* field, int iter);
    void gif(int iter);
};
