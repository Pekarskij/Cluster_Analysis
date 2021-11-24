#pragma once
#include "Cloud.h"
#include "Buffer.h"
#pragma warning (disable:4244)
#pragma warning (disable:26451)
#pragma warning (disable:6001)
#pragma warning (disable:4018)

class Field
{
private:
    int number_clouds_in_field, number_points_in_field;
    vector <Cloud> clouds_in_field;
    Buffer buffer;
public:
    //конструкторы и деструктор
    Field();
    Field(const Field& f);
    ~Field() = default;

    const Field& operator=(const Field& f);//операция присваивания

    //функции, связанные с атрибутами данного класса
    void add_cloud_in_field(Cloud c);
    void delete_cloud_from_field(int id);
    vector <Point> get_points_from_field();

    void print_field();//печать поля

    //буфер
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

    //гистограммы
    void creat_field_histogram(int number_of_columns);
    void creat_cloud_histogram(int id, int number_of_colunms);
};
