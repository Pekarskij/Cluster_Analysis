#include "kmcores.h"
#include "k_means.h"

kmcores::kmcores() { k = 0; p = 0; }

kmcores::kmcores(const kmcores& km) { k = km.k; p = km.p; }

const kmcores& kmcores::operator=(const kmcores& km) { k = km.k; p = km.p; return *this; }

void kmcores::assing_k(int k_k) { k = k_k; }
void kmcores::assing_p(int p_p) { p = p_p; }

Find_cluster kmcores::find_clusters(vector <Point>& points)
{
    double dist;
    int i, j, m, s , c, N = points.size();
    vector<vector<Point>> kmcores(k, vector<Point>(p));
    vector <Cluster> P(k);
    Find_cluster f;

    vector <Point> k_means(p);
    vector <int> n(p, 0);
    vector <double> sum_x(p, 0);
    vector <double> sum_y(p, 0);
    vector <Cluster> T(p);
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < N; j++) P[i].add_point_indicator(0);
    }
    for (i = 0; i < k; i++)//pervichnye centry
    {
        kmcores[i].push_back(points[i]);
        P[i].assign_point_indicator(i, 1);
    }
    for (i = 0; i < N; i++)
    {
        m = 0;
        dist = (points[i] - kmcores[0][0]).length();
        for (j = 0; j < k; j++)
        {
            if ((points[i] - kmcores[j][0]).length() < dist)
            {
                dist = (points[i] - kmcores[j][0]).length();
                m = j;
            }
        }
        P[m].assign_point_indicator(i, 1); //i-tay tochka poluchaet marker m
    }
    c = 1;
    while (c != 0)
    {
        c = 0;
        for (int t = 0; t < k; t++)
        {
            for (i = 0; i < p; i++)
            {
                n[i] = 0;
                sum_x[i] = 0;
                sum_y[i] = 0;
            }
             s = 1;
            for (i = 0; i < p; i++)
            {
                for (j = 0; j < N; j++)
                {
                    
                        T[i].add_point_indicator(0);
                }
            }
            for (i = 0; i < p; i++)
            {
                for (j = 0; j < N; j++)
                {
                    if (P[t].get_point_indicator(j) == 1)
                    {
                        k_means.push_back(points[j]);
                        T[i].assign_point_indicator(j, 1);
                        j = N;

                    }
                }
            }
            for (i = 0; i < N; i++)
            {
                if (P[t].get_point_indicator(i) == 1)
                {
                    m = 0;
                    dist = (points[i] - k_means[0]).length();
                    for (j = 0; j < p; j++)
                    {
                        if ((points[i] - k_means[j]).length() < dist)
                        {
                            dist = (points[i] - k_means[j]).length();
                            m = j;
                        }
                    }
                    T[m].assign_point_indicator(i, 1); //i-tay tochka poluchaet marker m
                }
            }
            while (s != 0)
            {
                s = 0;
                for (i = 0; i < N; i++)
                {
                    if (P[t].get_point_indicator(i) == 1) {
                        for (j = 0; j < p; j++)//schitaem centr tyazhesti
                        {
                            sum_x[j] = sum_x[j] + points[i].GetX() * T[j].get_point_indicator(i);
                            sum_y[j] = sum_y[j] + points[i].GetY() * T[j].get_point_indicator(i);
                            n[j] = n[j] + T[j].get_point_indicator(i);
                        }
                    }
                }
                k_means.clear();
                for (i = 0; i < p; i++)
                {
                    sum_x[i] = sum_x[i] / n[i];
                    sum_y[i] = sum_y[i] / n[i];
                    k_means.push_back(Point(sum_x[i], sum_y[i]));//novye centry
                    sum_x[i] = 0;
                    sum_y[i] = 0;
                    n[i] = 0;
                }
                for (i = 0; i < N; i++)
                {
                    if (P[t].get_point_indicator(i) == 1)
                    {
                        m = 0;
                        dist = (points[i] - k_means[0]).length();
                        for (j = 0; j < p; j++)
                        {
                            if ((points[i] - k_means[j]).length() < dist)
                            {
                                dist = (points[i] - k_means[j]).length();
                                m = j;
                            }
                        }
                        if (T[m].get_point_indicator(i) == 0)//tochka pomenyala marker
                        {
                            for (j = 0; j < p; j++) T[j].assign_point_indicator(i, 0);
                            T[m].assign_point_indicator(i, 1);
                            s++;
                        }
                    }
                    
                }
            }
            for (i = 0; i < p; i++)
            {
                kmcores[t][i] = k_means[i];
            }
            k_means.erase(k_means.begin(), k_means.end());
            T.erase(T.begin(), T.end());
        }
        for (i = 0; i < N; i++)
        {
                m = 0;
                dist = (points[i] - kmcores[0][0]).length();
                for (j = 0; j < k; j++)
                {
                    for (int t = 0; t < p; t++)
                    {
                        if ((points[i] - kmcores[j][t]).length() < dist)
                        {
                            dist = (points[i] - kmcores[j][t]).length();
                            m = j;
                        }
                    }
                }
                if (P[m].get_point_indicator(i) == 0)//tochka pomenyala marker
                {
                    for (j = 0; j < k; j++) P[j].assign_point_indicator(i, 0);
                    P[m].assign_point_indicator(i, 1);
                    c++;
                }
            
        }
        for (i = 0; i < k; i++)
        {
            kmcores[i].erase(kmcores[i].begin(), kmcores[i].end());
        }

    }

    for (i = 0; i < k; i++)
    {
        f.add_cluster(P[i]);
    }
    kmcores.clear();
    k_means.clear();
    P.clear();
    T.clear();
    return f;
}