#include "Field.h"
class delaunay {
private:
    vector<vector<bool>> Edge;
public:
    void delaunay_(vector <Point> points);
    void delaunay_base_run(vector<vector <int>>& baselines, vector <Point> points, vector<vector<int>>& triangles, int &iter);
   double delaunay_angle_to_edge(vector <Point>& edge, Point c);
   vector<vector<bool>> get_edge();
   void delaunay_fprintf(vector <Point> &points);
   void delaunay_fprintf_iter(int iteration, vector <Point> &points);
   void gif(int iter);
};