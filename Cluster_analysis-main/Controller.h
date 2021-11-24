#pragma once
#include "Field.h"
#include "Wave_algorithm.h"
#include "DBSCAN.h"
#include "k_means.h"
#include "kmcores.h"
#include "FOREL.h"
#include "EM.h"
#include "Spanning_tree.h"
#include "ha.h"
#include "Find_cluster.h"
#include "delaunay.h"
class Controller
{
private:
    Field field;
    bool analysis_state;
    vector <Find_cluster> find_clusters;
    int number_find_clusters_in_field;
public:
    Controller();
    Controller(const Controller& c);
    ~Controller() = default;
    const Controller& operator=(const Controller& c);
    void add_cloud_in_field(Cloud c);
    void delete_cloud_from_field(int id);
    void print_field();
    bool get_analysis_state();
    void add_cloud_from_field_in_buffer(int id);
    void add_all_clouds_from_field_in_buffer();
    void upload_cloud_from_buffer_in_field(int n);
    void upload_all_clouds_from_buffer_in_field();
    void shift_cloud_in_buffer(int n, Point v);
    void shift_all_clouds_in_buffer(Point v);
    void turn_cloud_in_bufffer(int n, double phi);
    void turn_all_clouds_in_bufffer(double phi);
    void compression_cloud_in_buffer(int n, double lambda);
    void compression_all_clouds_in_buffer(double lambda);
    void creat_field_histogram(int number_of_columns);
    void creat_cloud_histogram(int id, int number_of_colunms);
    void wave(double delta);
    void forel(double R);
    void dbscan(double delta, int k);
    void k_m(int k);
    void kmcore(int k, int p);
    void em(int k);
    void tree();
    void hierarchy(int k, int t);
    void tri_delaunay();
    void get_help(string& file_name);
    vector <Find_cluster> get_find_clusters();
};

