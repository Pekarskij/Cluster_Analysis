#pragma once
#include "Field.h"
#include "Find_cluster.h"
#define EPS 0.001

class EM {
    int k;
 public:
     EM();
     EM(const EM& em);
     const EM& operator=(const EM& em);
     ~EM() = default;
     void assign_k(int k_k);
     Find_cluster find_clusters(Field *field);
  double N (Point a, vector<double> m, vector<double> Sgm);
  void print_iter(vector <Cluster> P, Field* field, int iter);
  void gif(int iter);
  /*void em_fprintf (vector <Point> points,int iteration,
                   vector<vector<double>> sgm,
                   vector<vector<double>> m,
                   vector<vector<double>> r) const;*/
};

