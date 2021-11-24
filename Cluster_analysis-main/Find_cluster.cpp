#include "Find_cluster.h"

Find_cluster::Find_cluster() { number_clusters_in_find_cluster = number_points_in_find_cluster = delta = k = t = p = 0; }

Find_cluster::Find_cluster(const Find_cluster& fc)
{
    number_clusters_in_find_cluster = fc.number_clusters_in_find_cluster;
    number_points_in_find_cluster = fc.number_points_in_find_cluster;
    alg = fc.alg;
    alg_number = fc.alg_number;
    delta = fc.delta;
    k = fc.k;
    t = fc.t;
    p = fc.p;
    clusters = fc.clusters;
}

const Find_cluster& Find_cluster::operator=(const Find_cluster& fc)
{
    number_clusters_in_find_cluster = fc.number_clusters_in_find_cluster;
    number_points_in_find_cluster = fc.number_points_in_find_cluster;
    alg = fc.alg;
    alg_number = fc.alg_number;
    delta = fc.delta;
    k = fc.k;
    t = fc.t;
    p = fc.p;
    clusters = fc.clusters;
    return *this;
}

void Find_cluster::add_cluster(Cluster c)
{
    number_clusters_in_find_cluster++;
    number_points_in_find_cluster = number_points_in_find_cluster + c.get_number_points_in_cluster();
    clusters.push_back(c);
    clusters[number_clusters_in_find_cluster - 1].assign_cluster_id(number_clusters_in_find_cluster - 1);
}
 vector <double> Find_cluster::distance_matrix(vector <Point> points)
{
    int N = points.size();
    vector <double> D;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            D.push_back((points[i] - points[j]).length());
        }
    }
    return D;
}
vector <int> Find_cluster::graph_matrix(vector <Point> points, double delta)
{
    int N = points.size();
    vector <int> B;
    vector <double> D = distance_matrix(points);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (D[i*N + j] < delta) B.push_back(1);
            else B.push_back(0);
        }
    }
    return B;
}

int Find_cluster::get_number_clusters_in_find_cluster() { return number_clusters_in_find_cluster; }

int Find_cluster::get_number_points_in_find_cluster() { return number_points_in_find_cluster; }

int Find_cluster::get_point_indicator(int i, int j) { return clusters[i].get_point_indicator(j); }

void Find_cluster::assign_alg(string s) { alg = s; }

void Find_cluster::assign_alg_number(int n) { alg_number = n; }

void Find_cluster::assign_delta(double d) { delta = d; }

void Find_cluster::assign_k(int k_k) { k = k_k; }

void Find_cluster::assign_p(int p_p) { p = p_p; }

void Find_cluster::assign_t(int t_t) { t = t_t; }

int Find_cluster::get_alg_number() { return alg_number; }

string Find_cluster::get_alg() { return alg; }

double Find_cluster::get_delta() { return delta; }

int Find_cluster::get_k() { return k; }

int Find_cluster::get_p() { return p; }

int Find_cluster::get_t() { return t; }

Cluster Find_cluster::get_cluster(int id) { return clusters[id]; }

void Find_cluster::print_wave_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//Wave algorithm//Wave "),
        s_3("Algorithms//Wave algorithm//Wave ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'Wave " << s_1 << ".txt' palette title 'Wave, delta=" << delta << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}

void Find_cluster::print_dbscan_algorithm(vector <Point>& points, vector <int>& cpd)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//DBSCAN//DBSCAN "),
        s_3("Algorithms//DBSCAN//DBSCAN "),
        s_4("Algorithms//DBSCAN//Core "),
        s_5("Algorithms//DBSCAN//Peripheral "),
        s_6("Algorithms//DBSCAN//Dust "),
        s_7("Algorithms//DBSCAN//DBSCAN (core, peripheral, dust) ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    s_4.append(s_1);
    s_4.append(".txt");
    s_5.append(s_1);
    s_5.append(".txt");
    s_6.append(s_1);
    s_6.append(".txt");
    s_7.append(s_1);
    s_7.append(".plt");
    ofstream f(s_2), g(s_3), f_1(s_4), f_2(s_5), f_3(s_6);

    g << "set size square\n" << "set palette\n" << "plot 'DBSCAN " << s_1 << ".txt' palette title 'DBSCAN, delta=" << delta << ", k=" << k << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();

    g.open(s_7);
    for (int i = 0; i < points.size(); i++)
    {
        if (cpd[i] == 0) points[i].print_Point(f_1);
        if (cpd[i] == 1) points[i].print_Point(f_2);
        if (cpd[i] == 2) points[i].print_Point(f_3);
    }
    g << "set size square\n" << "plot 'Core " << s_1 << ".txt', 'Peripheral " << s_1 << ".txt', 'Dust " << s_1 << ".txt'\n";
    g.close();
    f_1.close();
    f_2.close();
    f_3.close();
}

void Find_cluster::print_k_means_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//k means//k means "),
        s_3("Algorithms//k means//k means ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'k means " << s_1 << ".txt' palette title 'k means, k=" << k << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}

void Find_cluster::print_em_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//em//em "),
        s_3("Algorithms//em//em ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);

    g << "set size square\n" << "set palette\n" << "plot 'EM " << s_1 << ".txt' palette title 'em, k=" << k << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}
void Find_cluster::print_kmcores_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//kmcores//kmcores "),
        s_3("Algorithms//kmcores//kmcores ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'kmcores " << s_1 << ".txt' palette title 'kmcores, k=" << k << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}
void Find_cluster::print_forel_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//forel//forel "),
        s_3("Algorithms//forel//forel ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'forel " << s_1 << ".txt' palette title 'forel, R=" << delta << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}

void Find_cluster::print_ha_algorithm(vector <Point>& points)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//ha//ha "),
        s_3("Algorithms//ha//ha ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'ha " << s_1 << ".txt' palette title 'ha, k=" << k << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, points);
    f.close();
    g.close();
}