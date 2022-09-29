#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <string>

using namespace std;

class Point
{
private:
    double X, Y, function_value;
    int Point_id;
public:
    Point();
    Point(double x, double y);
    Point(const Point& v);
    ~Point() = default;
    const Point& operator=(const Point& v);
    friend Point operator+(const Point& v, const Point& u);
    friend Point operator-(const Point& v, const Point& u);
    friend Point operator*(double lambda, const Point& v);
    friend Point operator*(const Point& v, double lambda);
    double length();
    double GetX();
    double GetY();
    double get_function_value();
    int get_Point_id();
    void assign_Point_id(int id);
    void print_Point(ofstream& file);
};

