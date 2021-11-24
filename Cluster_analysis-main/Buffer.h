#pragma once
#include "Cloud.h"

class Buffer
{
private:
    int number_clouds_in_buffer;
     vector <Cloud> clouds_in_buffer;
public:
    Buffer();
    Buffer(const Buffer& b);
    ~Buffer() = default;

    const Buffer& operator=(const Buffer& b);


    void add_cloud_in_buffer(Cloud c);
    Cloud upload_cloud_from_buffer(int n);
    int get_number_clouds_in_buffer();

    //preobrazovanie oblakov
    void shift_cloud_in_buffer(int n, Point v);
    void shift_all_clouds_in_buffer(Point v);
    void turn_cloud_in_buffer(int n, double phi);
    void turn_all_clouds_in_buffer(double phi);
    void compression_cloud_in_buffer(int n, double lambda);
    void compression_all_clouds_in_buffer(double lambda);
};

