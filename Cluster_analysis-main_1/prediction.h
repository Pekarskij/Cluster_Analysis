#pragma once
#include "Find_cluster.h"
#include "Field.h"
class prediction
{
private:
	double R;//oshibka
	double f;
public:
	prediction();
	~prediction() = default;
	int interpolation(Point p, Field *field);
	void find_R(Field *field);
	double get_R();
	double get_f();
	double Gauss(double x);
	//void delaunay_fprintf(vector <Point> points, int k, vector<vector<bool>> ed);
};
