#include "Wave_algorithm.h"

Wave_algorithm::Wave_algorithm() { delta = 0; }

Wave_algorithm::Wave_algorithm(const Wave_algorithm& alg) { delta = alg.delta; }

const Wave_algorithm& Wave_algorithm::operator=(const Wave_algorithm& alg) { delta = alg.delta; return *this; }

void Wave_algorithm::assign_delta(double d) { delta = d; }

double Wave_algorithm::get_delta() { return delta; }

Find_cluster Wave_algorithm::find_clusters(Field *field)
{
    int i, j, k, t = 0, c, n = field->get_number_points_in_field(), s = 0;
    vector <vector <int>> point_indicators(n);
    vector <Cluster> C;
    Find_cluster fc;
    vector <double> D = field->get_dist_mat();
    fc.create_binary(field, delta);
    vector <int> B = fc.get_binary();

    for (i = 0; i < n; i++)
    {
        point_indicators[i].push_back(0);
        point_indicators[i].push_back(0);
        point_indicators[i].push_back(0);
    }
    for (i = 0; i < n; i++)//point_indicators[i][2] == 1/0 - i-tay tochka v clustere/ne v clustere, point_indicators[i][0] - metka i-toy tochki
    {
        if (point_indicators[i][2] == 0)
        {
            Cluster cl;
            t = 1;
            point_indicators[i][0] = t;//podjog
            point_indicators[i][2] = 1;
            for (j = 0; j < n; j++) cl.add_point_indicator(0);
            cl.assign_point_indicator(i, 1);
            C.push_back(cl);
            c = 1;
            while (c != 0)
            {
                c = 0;
                for (j = 0; j < n; j++)
                {
                    if (point_indicators[j][0] == t)//esli tochka prinadlezhit frontu v moment vremeni t+1
                    {
                        for (k = 0; k < n; k++)
                        {
                            if (k == j) point_indicators[k][1] = t; //metka tochki v sleduyushchij moment vremeni
                            if (B[j*n + k] == 1 && point_indicators[k][2] == 0 && point_indicators[k][0] == 0)
                            {
                                point_indicators[k][1] = t + 1;//metka tochki v sleduyushchij moment vremeni
                                point_indicators[k][2] = 1;
                                C[s].assign_point_indicator(k, 1);
                                c++;
                            }
                        }
                    }
                }
                for (j = 0; j < n; j++) point_indicators[j][0] = point_indicators[j][1];
                t++;
            }
            s++;
        }
    }
    int q = C.size();
    for (i = 0; i < q; i++) fc.add_cluster(C[i]);
    return fc;
}
