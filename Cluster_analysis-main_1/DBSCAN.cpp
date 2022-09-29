#include "DBSCAN.h"

DBSCAN::DBSCAN() { delta = k = 0; }

DBSCAN::DBSCAN(const DBSCAN& alg) { delta = alg.delta; k = alg.k; }

const DBSCAN& DBSCAN::operator=(const DBSCAN& alg) { delta = alg.delta; k = alg.k; return *this; }

void DBSCAN::assign_delta(double d_d) { delta = d_d; }

void DBSCAN::assign_k(int k_k) { k = k_k; }

double DBSCAN::get_delta() { return delta; }

int DBSCAN::get_k() { return k; }

Find_cluster DBSCAN::find_clusters(Field *field, vector <int>& cpd) //cpd = 0 -osnovnaya tochka, cpd = 1 - tochka periferii, cpd = 2 - shum
{
    vector<Point> points = field->get_points_from_field();
    int i, j, l, t, c, N = 0, n = field->get_number_points_in_field(), s = 0;
   vector <double>  D_core;
   vector <int>  B_core;
    vector <Point> core_points;
    vector <int> core_points_indicators;
    vector <vector <int>> point_indicators;
    vector <Cluster> C;
    Find_cluster fc;

    cpd.resize(n);
    vector <double> D = field->get_dist_mat();
    fc.create_binary(field, delta);
    vector <int> B = fc.get_binary();
    
    for (i = 0; i < n; i++) cpd[i] = 2;
    for (i = 0; i < n; i++)
    {
        l = 0;
        for (j = 0; j < n; j++)
        {
            if (B[i*n + j] == 1) l++;
        }
        l--;
        if (l > k - 1) cpd[i] = 0;//esli sosedej bol'she ili ravno k, to tochka osnovnaya
    }

    for (i = 0; i < n; i++)
    {
        if (cpd[i] == 2)
        {
            for (j = 0; j < n; j++)
            {
                if (B[i*n + j] == 1 && cpd[j] == 0)
                {
                    cpd[i] = 1;//esli est' sosednyaya osnovnaya tochka, to dannaya tochka periferijnaya
                    j = n - 1;
                }
            }
        }
    }
  
    for (i = 0; i < n; i++)
    {
        Cluster cl;
        for (j = 0; j < n; j++) cl.add_point_indicator(0);
        if (cpd[i] == 2)//Esli tochka - shum, to eto klaster iz odnoj tochki
        {
            cl.assign_point_indicator(i, 1);
            C.push_back(cl);
            s++;
        }
        else
        {
            core_points.push_back(points[i]);
            core_points_indicators.push_back(i);
            N++;
        }
    }
    D_core = fc.distance_matrix(core_points);
    B_core = fc.graph_matrix(core_points, delta);
    point_indicators.resize(N);
   
    for (i = 0; i < N; i++)//volnovoi algoritm dlya osnovnyh i periferijnyh tochek
    { 
        //point_indicators[i][2] == 1/0 - i-tay tochka v clustere/ne v clustere, point_indicators[i][0] - metka i-toy tochki
        point_indicators[i].push_back(0);
        point_indicators[i].push_back(0);
        point_indicators[i].push_back(0);
    }
    for (i = 0; i < N; i++)
    {
        if (point_indicators[i][2] == 0)
        {
            
           Cluster cl;
            t = 1;
            point_indicators[i][0] = t;//podjog
            point_indicators[i][2] = 1;
            for (j = 0; j < n; j++) cl.add_point_indicator(0);
            cl.assign_point_indicator(core_points_indicators[i], 1);
            C.push_back(cl);
            c = 1;
            while (c != 0)
            {
              
                c = 0;
                for (j = 0; j < N; j++)
                {
                    if (point_indicators[j][0] == t)//esli tochka prinadlezhit frontu v moment vremeni t+1
                    {
                        for (l = 0; l < N; l++)
                        {
                            if (l == j) point_indicators[l][1] = t; //metka tochki v sleduyushchij moment vremeni
                            if (B_core[j*N + l] == 1 && point_indicators[l][2] == 0 && point_indicators[l][0] == 0)
                            {
                                point_indicators[l][1] = t + 1; //metka tochki v sleduyushchij moment vremeni
                                point_indicators[l][2] = 1;
                                C[s].assign_point_indicator(core_points_indicators[l], 1);
                                c++;
                            }
                        }
                    }
                }
                for (j = 0; j < N; j++) point_indicators[j][0] = point_indicators[j][1];
                t++;
            }
            
            s++;
        }
    }
    int q = C.size();
    for (i = 0; i < q; i++) fc.add_cluster(C[i]);
    D.clear();
    D_core.clear();
     B.clear();
     B_core.clear();
     core_points.clear();
    core_points_indicators.clear();
   point_indicators.clear();
    C.clear();
    return fc;
}
