#include "Point.h"

Point::Point()
{
    X = Y = function_value = Point_id = 0;
}

Point::Point(double x, double y)
{
    X = x;
    Y = y;
    function_value = (X * X - Y * Y) / 10 + 5;
    Point_id = 0;
}

Point::Point(const Point& v)
{
    X = v.X;
    Y = v.Y;
    function_value = v.function_value;
    Point_id = v.Point_id;
}

Point operator+(const Point& v, const Point& u)
{
    return Point(v.X + u.X, v.Y + u.Y);
}

Point operator-(const Point& v, const Point& u)
{
    return Point(v.X - u.X, v.Y - u.Y);
}

Point operator*(double lambda, const Point& v)
{
    return Point(lambda * v.X, lambda * v.Y);
}

Point operator*(const Point& v, double lambda)
{
    return Point(lambda * v.X, lambda * v.Y);
}

const Point& Point::operator=(const Point& v)
{
    X = v.X;
    Y = v.Y;
    function_value = v.function_value;
    Point_id = v.Point_id;
    return *this;
}

double Point::length() { return sqrt(X * X + Y * Y); }

double Point::GetX() { return X; }

double Point::GetY() { return Y; }

double Point::get_function_value() { return function_value; }

int Point::get_Point_id() { return Point_id; }

void Point::assign_Point_id(int id) { Point_id = id; }

void Point::print_Point(ofstream& file) { file << X << " " << Y << endl; }