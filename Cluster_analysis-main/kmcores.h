#pragma once
#include "Find_cluster.h"

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
    Find_cluster find_clusters(vector <Point>& points);
};
