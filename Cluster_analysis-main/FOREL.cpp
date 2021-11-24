#include "FOREL.h"

FOREL::FOREL() { R = 0; }

FOREL::FOREL(const FOREL& f) { R = f.R; }

const FOREL& FOREL::operator=(const FOREL& f) { R = f.R;  return *this; }

void FOREL::assign_R(double r) { R = r; }


double FOREL::get_R() { return R; }


Find_cluster FOREL::find_clusters(vector <Point>& points)
{
    Point cent;
    int i, j, n = points.size(), c=1, s = 0, t, N;
    double sum_x, sum_y;
    vector <int> ind(n, 0);
    vector <Cluster> C;
    Find_cluster fc;

    for (i = 0; i < n; i++) { ind.push_back(0); }

        while (c != 0)
        {
            Cluster cl;
            for (j = 0; j < n; j++) cl.add_point_indicator(0);
            C.push_back(cl);
            c = 0;
            for (i = 0; i < n; i++)
            {
                if (ind[i] == 0)
                {
                    cent = points[i];
                    i = n;
                }
            }
            for (i = 0; i < n; i++)
            {
                if (((points[i] - cent).length()) <= R && (ind[i] != 2)) ind[i] = 1;
            }
            t = 1;
            while (t != 0)
            {
                t = 0;
                sum_x = 0;
                sum_y = 0;
                N = 0;
                for (i = 0; i < n; i++)
                {
                    if (ind[i] == 1)
                    {
                        sum_x = sum_x + points[i].GetX();
                        sum_y = sum_y + points[i].GetY();
                        N++;
                    }
                }
                sum_x = sum_x / N;
                sum_y = sum_y / N;
                if (((cent.GetX()) != sum_x) || ((cent.GetY()) != sum_y))
                {
                    cent = Point(sum_x, sum_y);
                    t++;
                    for (i = 0; i < n; i++)
                    {
                        if (ind[i] != 2) ind[i]=0;
                    }
                    for (i = 0; i < n; i++)
                    {
                        if ((points[i] - cent).length() <= R && ind[i] != 2) ind[i] = 1;
                    }
                }
            }
            for (i = 0; i < n; i++)
            {
                if (ind[i] != 2) ind[i] = 0;
                if ((points[i] - cent).length() <= R && ind[i] != 2)
                {
                    ind[i] = 2;
                    C[s].assign_point_indicator(i, 1);
                }
            }
            s++;
            for (i = 0; i < n; i++)
            {
                if (ind[i] == 0) c++;
            }
        }
        for (i = 0; i < C.size(); i++) fc.add_cluster(C[i]);
        return fc;
}
