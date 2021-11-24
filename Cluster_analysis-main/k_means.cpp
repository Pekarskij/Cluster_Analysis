#include "k_means.h"

k_means::k_means() { k = 0; }

k_means::k_means(const k_means& km) { k = km.k; }

const k_means& k_means::operator=(const k_means& km) { k = km.k; return *this; }

void k_means::assing_k(int k_k) { k = k_k; }

Find_cluster k_means::find_clusters(vector <Point>& points)
{
    double dist;
    int i, j, m, c, N = points.size();
    vector <Point> k_means;
    vector <int> n(k, 0);
    vector <double> sum_x(k, 0);
    vector <double> sum_y(k, 0);
    vector <Cluster> P(k);
    Find_cluster f;
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < N; j++) P[i].add_point_indicator(0);
    }
    for (i = 0; i < k; i++)//pervichnye centry
    {
        k_means.push_back(points[i]);
        P[i].assign_point_indicator(i, 1);
    }
    for (i = 0; i < N; i++)
    {
        m = 0;
        dist = (points[i] - k_means[0]).length();
        for (j = 0; j < k; j++)
        {
            if ((points[i] - k_means[j]).length() < dist)
            {
                dist = (points[i] - k_means[j]).length();
                m = j;
            }
        }
        P[m].assign_point_indicator(i, 1); //i-tay tochka poluchaet marker m
    }
    c = 1;
    while (c != 0)
    {
        c = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < k; j++)//schitaem centr tyazhesti
            {
                sum_x[j] = sum_x[j] + points[i].GetX() * P[j].get_point_indicator(i);
                sum_y[j] = sum_y[j] + points[i].GetY() * P[j].get_point_indicator(i);
                n[j] = n[j] + P[j].get_point_indicator(i);
            }
        }
        k_means.clear();
        for (i = 0; i < k; i++)
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
            m = 0;
            dist = (points[i] - k_means[0]).length();
            for (j = 0; j < k; j++)
            {
                if ((points[i] - k_means[j]).length() < dist)
                {
                    dist = (points[i] - k_means[j]).length();
                    m = j;
                }
            }
            if (P[m].get_point_indicator(i) == 0)//tochka pomenyala marker
            {
                for (j = 0; j < k; j++) P[j].assign_point_indicator(i, 0);
                P[m].assign_point_indicator(i, 1);
                c++;
            }
        }
    }
    for (i = 0; i < k; i++)
    {
        P[i].assign_cluster_center(k_means[i]);
        f.add_cluster(P[i]);
    }
    return f;
}