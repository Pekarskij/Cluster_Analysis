#pragma once
#include "Point.h"

class Cloud
{
private:
    double mean_value_for_X, mean_value_for_Y, dispersion_for_X, dispersion_for_Y, radius, diameter;
    int number_points_in_cloud, cloud_id;
    vector <Point> points_in_cloud;
public:
    
    Cloud();
    Cloud(double mean_x, double mean_y, double sigma_x, double sigma_y, int n);
    Cloud(const Cloud& c);
    ~Cloud() = default;

    const Cloud& operator=(const Cloud& c);

    
    Point get_cloud_center();
    Point get_dispersions();
    double get_radius();
    double get_diameter();
    int get_number_points_in_cloud();
    int get_cloud_id();
    Point get_point_in_cloud(int point_id);
    void assign_cloud_id(int id);
    void assign_point_id_in_cloud(int n, int id);

    void print_cloud(ofstream& file);

    void creat_cloud_histogram(int number_of_columns);

    
    void shift_cloud(Point v);
    void turn_cloud(double phi);
    void compression_cloud(double lambda);
};