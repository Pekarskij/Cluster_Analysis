#pragma once
#include "Cluster.h"
#include "Field.h"
class Find_cluster
{
private:
    int number_clusters_in_find_cluster, number_points_in_find_cluster, alg_number;
    vector <Cluster> clusters;
    string alg;
    double delta;
    int k;
    int p;
    int t;
    vector<int> binary;
public:
    Find_cluster();
    Find_cluster(const Find_cluster& fc);
    ~Find_cluster() = default;

    const Find_cluster& operator=(const Find_cluster& fc);


    void add_cluster(Cluster c);
    int get_number_clusters_in_find_cluster();
    int get_number_points_in_find_cluster();
    int get_point_indicator(int i, int j);
    void assign_alg(string s);
    void assign_alg_number(int n);
    void assign_delta(double d);
    void assign_k(int k_k);
    void assign_p(int p_p);
    void assign_t(int t_t);
    string get_alg();
    int get_alg_number();
    double get_delta();
    int get_k();
    int get_p();
    int get_t();
    Cluster get_cluster(int id);
    vector <double> distance_matrix(vector <Point> points);
    void create_binary(Field* field, double delta);
    vector<int> get_binary();
    vector <int> graph_matrix(vector <Point> points, double delta);
    void print_wave_algorithm(Field *field);
    void print_dbscan_algorithm(Field *field, vector <int>& cpd);
    void print_k_means_algorithm(Field *field);
    void print_em_algorithm(Field *field);
    void print_kmcores_algorithm(Field *field, vector<vector<Point>>& cores);
    void print_forel_algorithm(Field *field);
    void print_ha_algorithm(Field *field);
    void print_binary_algorithm(Field* field);
};
