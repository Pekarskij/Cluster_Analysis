#include "Field.h"
class delaunay {
    void delaunay_fprintf(vector<vector<bool>>& edge,  vector <Point> points);
public:
    void delaunay_(vector <Point> points);
    void delaunay_base_run(vector<vector <Point>>& baselines,
        vector<vector<bool>>& edge, vector <Point> points);
   double delaunay_angle_to_edge(vector <Point>& edge, Point c);
};