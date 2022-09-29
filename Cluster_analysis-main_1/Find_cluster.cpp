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
    binary = fc.binary;
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
    binary = fc.binary;
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
void Find_cluster::create_binary(Field *field, double delta)
{
    int N = field->get_number_points_in_field();
    vector <double> D = field->get_dist_mat();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (D[i*N + j] < delta) binary.push_back(1);
            else binary.push_back(0);
        }
    }
    D.clear();
}
vector<int> Find_cluster::get_binary()
{
    return binary;
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
            if (D[i * N + j] < delta) B.push_back(1);
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

void Find_cluster::print_wave_algorithm(Field *field)
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
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    f.close();
    g.close();
}

void Find_cluster::print_dbscan_algorithm(Field *field, vector <int>& cpd)
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
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    f.close();
    g.close();
    vector<Point> points = field->get_points_from_field();
    g.open(s_7);
    int q = points.size();
    for (int i = 0; i < q; i++)
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
    points.clear();
}

void Find_cluster::print_k_means_algorithm(Field *field)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//k means//k means "),
        s_3("Algorithms//k means//k means "),
        c("Algorithms//k means//k means_centers ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    c.append(s_1);
    c.append(".txt");
    ofstream f(s_2), g(s_3), h(c);
    g << "set size square\n" << "set palette\n" << "plot 'k means " << s_1 << ".txt' palette notitle"  << ",\\" << "\n" << "'k means_centers " << s_1 << ".txt' w p pt 15 ps 2 lc rgb 'black' title 'k means, k=" << k <<"\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    for (int i = 0; i < number_clusters_in_find_cluster; i++)
    {
        h << clusters[i].get_cluster_center().GetX() << " " << clusters[i].get_cluster_center().GetY() << "\n";
    }
    f.close();
    g.close();
    h.close();
}

void Find_cluster::print_em_algorithm(Field *field)
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
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    f.close();
    g.close();
}
void Find_cluster::print_kmcores_algorithm(Field *field, vector<vector<Point>>& cores)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//kmcores//kmcores "),
        s_3("Algorithms//kmcores//kmcores "),
    c("Algorithms//kmcores//kmcores_cores ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    c.append(s_1);
    c.append(".txt");

    ofstream f(s_2), g(s_3), h(c);
    g << "set size square\n" << "set palette\n" << "plot 'kmcores " << s_1 << ".txt' palette notitle" << ",\\" << "\n" << "'kmcores_cores " << s_1 << ".txt' w p pt 15 ps 2 lc rgb 'black' title 'kmcores, k=" << k << ", p=" << p << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    for (int i = 0; i < number_clusters_in_find_cluster; i++)
    {
        for (int j = 0; j < p; j++)

        {
            h << cores[i][j].GetX() << " " << cores[i][j].GetY() << "\n";
        }

    }
    f.close();
    g.close();
    h.close();
}
void Find_cluster::print_forel_algorithm(Field *field)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//forel//forel "),
        s_3("Algorithms//forel//forel "),
        c("Algorithms//forel//forel_centers ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    c.append(s_1);
    c.append(".txt");
    ofstream f(s_2), g(s_3), h(c);
    g << "set size square\n" << "set palette\n" << "plot 'forel " << s_1 << ".txt' palette notitle" << ",\\" << "\n" << "'forel_centers " << s_1 << ".txt' w p pt 15 ps 2 lc rgb 'black' title 'forel, R=" << delta << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    for (int i = 0; i < number_clusters_in_find_cluster; i++)
    {
        h << clusters[i].get_cluster_center().GetX() << " " << clusters[i].get_cluster_center().GetY() << "\n";
    }
    f.close();
    g.close();
    h.close();
}

void Find_cluster::print_ha_algorithm(Field *field)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//ha//ha "),
        s_3("Algorithms//ha//ha ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'ha " << s_1 << ".txt' palette title 'ha, k=" << k << ", t=" << t << "\n";
    for (int i = 0; i < number_clusters_in_find_cluster; i++) clusters[i].print_cluster(f, field);
    f.close();
    g.close();
}
void Find_cluster::print_binary_algorithm(Field* field)
{
    string s_1 = to_string(alg_number),
        s_2("Algorithms//Binary//Binary "),
        s_3("Algorithms//Binary//Binary ");
    s_2.append(s_1);
    s_2.append(".txt");
    s_3.append(s_1);
    s_3.append(".plt");
    ofstream f(s_2), g(s_3);
    g << "set size square\n" << "set palette\n" << "plot 'Binary " << s_1 << ".txt' with lines" << "\n";
    int n = field->get_number_points_in_field();
    vector<Point> points = field->get_points_from_field();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (binary[i*n +j]) {
                f << points[i].GetX() << " " << points[i].GetY() << endl;
                f << points[j].GetX() << " " << points[j].GetY() << endl << endl;
            }
        }
    }
    f.close();
    g.close();
    points.clear();
}
