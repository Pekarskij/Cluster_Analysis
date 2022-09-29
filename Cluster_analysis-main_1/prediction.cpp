#include "prediction.h"
#include "k_means.h"
#include "delaunay.h"
prediction::prediction()
{
    R = 0;
    f = 0;
}
double prediction::get_R()
{
    return R;
}
double prediction::get_f()
{
    return f;
}

int prediction::interpolation(Point p, Field *field)
{
    vector<Point> points = field->get_points_from_field();
    Find_cluster F;
    k_means alg;
    vector <Point> cluster;
    int i, k;
    double dist = 100, sum_w = 0, sum_w_f = 0;
    delaunay T;
   // int N = field->get_number_points_in_field();
    alg.assing_k(5);
    F = alg.find_clusters(field);//ishchem klastery

    for (i = 0; i < F.get_number_clusters_in_find_cluster(); i++)//ishchem blizhajshij klaster k tochke
    {
        if ((p - F.get_cluster(i).get_cluster_center()).length() < dist)
        {
            dist = (p - F.get_cluster(i).get_cluster_center()).length();
            k = i;
        }
    }
    if (dist > 2)
    {
        return -2;
        //cout << "Error. Points is too far away from the clusters.\n\n";
    }
    else
    {
        int q = points.size();
        for (i = 0; i < q; i++)
        {
            if (F.get_cluster(k).get_point_indicator(i) == 1) cluster.push_back(points[i]);
        }
        int n = F.get_cluster(k).get_number_points_in_cluster();
        cluster.push_back(p);
        T.delaunay_(cluster);//trianguliruem blizhajshij klaster
        vector<vector<bool>> edge = T.get_edge();

        for (i = 0; i < n; i++)//schitaem priblizhyonnoe znachenie funkcii po sosednim tochkam
        {
            if (edge[n][i])
            {
                sum_w_f = sum_w_f + cluster[i].get_function_value() * (p - cluster[i]).length();
                sum_w = sum_w + (p - cluster[i]).length();
            }
        }
        f = sum_w_f / sum_w;//priblizhyonnoe znachenie
        cluster.clear();
        points.clear();
        return 0;
    }
}

void prediction::find_R(Field *field)
{
    vector<Point> points = field->get_points_from_field();
    Find_cluster F;
    k_means alg;
    vector <Point> cluster;
    int i, j, k, N, n;
    double  sum_w = 0, sum_w_f = 0, sum_f = 0, sum_eps = 0, mean_f = 0;
    delaunay Tri;
    vector <double> w, eps;
    N = field->get_number_points_in_field();
    alg.assing_k(5);
    F = alg.find_clusters(field);//ishchem klastery


    for (i = 0; i < N; i++) mean_f = mean_f + points[i].get_function_value();
    mean_f = mean_f * (1 / N);//srednee znachenie
    vector<vector<vector<bool>>> edges;
    vector<vector<Point>> clusters;


    //trianguliruem kazhdyj klaster
    for (i = 0; i < F.get_number_clusters_in_find_cluster(); i++)
    {
        for (j = 0; j < N; j++)
        {
            if (F.get_cluster(i).get_point_indicator(j) == 1)
            {
                cluster.push_back(points[j]);

            }

        }
        clusters.push_back(cluster);
        Tri.delaunay_(cluster);
        edges.push_back(Tri.get_edge());
        cluster.clear();
        //delaunay_fprintf(clusters[i], i, edges[i]);
    }



    for (int m = 0; m < N; m++)//predskazyvaem znachenie kazhdoj tochki polya po eyo sosedyam
    {
        sum_w = 0;
        sum_w_f = 0;

        for (i = 0; i < F.get_number_clusters_in_find_cluster(); i++)//ishchem v kakoj klaster popadaet tochka
        {
            if (F.get_point_indicator(i, m) == 1)
            {

                k = i;
                break;
            }
        }

        int t = 0;
        for (i = 0; i < N; i++)//ishchem kakaya po schyotu dannaya tochka v klastere
        {
            if (F.get_cluster(k).get_point_indicator(i) == 1)
            {
                if (i != m)
                {
                    t++;
                }
                else
                {
                    break;
                }
            }
        }

        n = F.get_cluster(k).get_number_points_in_cluster();
        for (j = 0; j < n; j++)//schitaem priblizhyonnoe znachenie
        {
            if (edges[k][j][t] || edges[k][t][j])
            {
                //if ((clusters[k][j] - points[m]).length() > 0.0001)
                //{
                sum_w_f = sum_w_f + clusters[k][j].get_function_value()*(clusters[k][j] - points[m]).length();
                    sum_w = sum_w + (clusters[k][j] - points[m]).length();
                //}
            }
        }

        eps.push_back(points[m].get_function_value() - (sum_w_f / sum_w));//vektor oshibok
        if (isnan(points[m].get_function_value() - (sum_w_f / sum_w))) {
            cout << points[m].GetX() << " " << points[m].GetY() << " " << n << " " << t << " " << k << endl;
        }
       // printf("%f\n", points[m].get_function_value() - (sum_w_f / sum_w));
        cluster.clear();
    }
    for (i = 0; i < N; i++) {
        sum_f = sum_f + ((points[i].get_function_value() - mean_f) * (points[i].get_function_value() - mean_f));
        sum_eps = sum_eps + eps[i] * eps[i];
    }
    R = 1 - sum_eps / sum_f;//koefficient oshibki
    w.clear();
    eps.clear();
    edges.clear();
    clusters.clear();
}
double prediction::Gauss(double x) { return exp(-(x * x) / 2); }
//void prediction::delaunay_fprintf(vector <Point> points, int k, vector<vector<bool>> ed) {
//    // prints algorithm's state
//    ofstream edges("dl " + to_string(k) + ".txt"), g("dl " + to_string(k) + ".plt");
//    for (int i = 0; i < points.size(); ++i) {
//        for (int j = i + 1; j < points.size(); ++j) {
//            if (ed[i][j]) {
//                edges << points[i].GetX() << " " << points[i].GetY() << " " << i << endl;
//                edges << points[j].GetX() << " " << points[j].GetY() <<" "<< j << endl << endl;
//            }
//        }
//    }
//    g << "set size square\n" << "set palette\n" << "plot 'dl "<<k<<".txt' with lines\n";
//}
