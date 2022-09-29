#include "FOREL.h"

FOREL::FOREL() { R = 0; }

FOREL::FOREL(const FOREL& f) { R = f.R; }

const FOREL& FOREL::operator=(const FOREL& f) { R = f.R;  return *this; }

void FOREL::assign_R(double r) { R = r; }


double FOREL::get_R() { return R; }


Find_cluster FOREL::find_clusters(Field *field)
{
    vector<Point> points = field->get_points_from_field();
    Point cent;
    int i, j, n = field->get_number_points_in_field(), c=1, s = 0, t, N;
    double sum_x, sum_y;
    vector <int> ind(n, 0);
    vector <Cluster> C;
    Find_cluster fc;

    for (i = 0; i < n; i++) { ind.push_back(0); }//ind[i]=2 - i-aya tochka uzhe v klastere, ind[i]=1 - i-aya tochka v R-okrestnosti, ind[i]=0 - i-aya tochka ni v klastere, ni v R-okrestnosti

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
                    cent = points[i];//pervichnyj centr
                    i = n;
                }
            }
            for (i = 0; i < n; i++)
            {
                if (((points[i] - cent).length()) <= R && (ind[i] != 2)) ind[i] = 1;//i-aya tochka v R-okrestnosti
            }
            t = 1;

            while (t != 0)//povtoryaem poka centr ne stanet nepodvizhnym
            {
                t = 0;
                sum_x = 0;
                sum_y = 0;
                N = 0;
                for (i = 0; i < n; i++)//schitaem centr tyazhesti
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
                double d_x = cent.GetX() - sum_x;
                double d_y = cent.GetY() - sum_y;
                if(d_x <0)
                {
                    d_x = - d_x;
                }
                if(d_y < 0)
                {
                    d_y = - d_y;
                }
                if ((d_x > 0.01) || (d_y > 0.01))
                {
                    cent = Point(sum_x, sum_y);//novyj centr
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

            for (i = 0; i < n; i++)//tochki v R-okrestnosti obrazuyut klaster
            {
                if (ind[i] != 2) ind[i] = 0;
                if ((points[i] - cent).length() <= R && ind[i] != 2)
                {
                    ind[i] = 2;
                    C[s].assign_point_indicator(i, 1);
                }
                
            }
            C[s].assign_cluster_center(cent);
            s++;
            for (i = 0; i < n; i++)
            {
                if (ind[i] == 0) c++;//esli ostalis' eshchyo tochki, to povtoryaem cikl
            }
        }
        int q = C.size();
        for (i = 0; i < q; i++) fc.add_cluster(C[i]);
        return fc;
}
