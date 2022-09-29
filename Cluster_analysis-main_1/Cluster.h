#pragma once
#include "Point.h"
#include "Field.h"
class Cluster
{
private:
    int number_points_in_cluster, cluster_id;
    vector <int> point_indicators; //point_indicators[i]=1 - i-tay tochka polya v clustere, point_indicators[i]=0 - i-tay tochka polya ne v clustere
    Point cluster_center;
    double radius, diameter;
public:
    Cluster();
    Cluster(const Cluster& c);
    ~Cluster() = default;

    const Cluster& operator=(const Cluster& c);


    int get_number_points_in_cluster();
    int get_cluster_id();
    int get_point_indicator(int id);
    Point get_cluster_center();
    double get_radius();
    double get_diameter();
    void assign_cluster_id(int id);
    void assign_cluster_center(Point p);
    void add_point_indicator(int n);
    void assign_point_indicator(int id, int n);

    void print_cluster(ofstream& file, Field *field);


    void find_cluster_center(vector <Point>& points);
    void find_radius_and_diameter(vector <Point>& points);
};

