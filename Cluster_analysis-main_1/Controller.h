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
#include "prediction.h"
class Controller
{
private:
    Field *field;
    bool analysis_state;
    string log_file_name;
    ofstream logger;
    bool write_log;
    void log(const string& s);
    vector <Find_cluster> find_clusters;
    int number_find_clusters_in_field;
    Wave_algorithm w;
    FOREL f;
    DBSCAN db;
    k_means km;
    kmcores kmc;
    EM em;
    Spanning_tree T;
    ha h;
    delaunay tri;
    prediction Prediction;
public:
    Controller(bool write_log);
    //Controller(const Controller& c);
    ~Controller();
   // const Controller& operator=(const Controller& c);
    int add_cloud_in_field(double x, double y, double disp_x, double disp_y, int dots);
   // void delete_cloud_from_field(int id);
    int print_field();
    bool get_analysis_state();
    int add_cloud_from_field_in_buffer(int id);
    int add_all_clouds_from_field_in_buffer();
   // void upload_cloud_from_buffer_in_field(int n);
    int upload_all_clouds_from_buffer_in_field();
   // void shift_cloud_in_buffer(int n, Point v);
    int shift_all_clouds_in_buffer(Point v);
    //void turn_cloud_in_bufffer(int n, double phi);
    int turn_all_clouds_in_bufffer(double phi);
   // void compression_cloud_in_buffer(int n, double lambda);
    int compression_all_clouds_in_buffer(double lambda);
    int creat_field_histogram(int number_of_columns);
   // void creat_cloud_histogram(int id, int number_of_colunms);

    int matrix();
    int binary(double delta);
    int wave(double delta);
    int forel(double R);
    int dbscan(double delta, int k);
    int k_m(int k);
    int kmcore(int k, int p);
    int em_(int k);
    int tree();
    int hierarchy(int k, int t);
    int tri_delaunay();
    int function_interpolation(Point p, double &f);
    int get_help(string& file_name);
    int get_find_clusters(vector <Find_cluster>& fc);
};

