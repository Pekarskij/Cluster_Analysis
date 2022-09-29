#pragma once
#include "Find_cluster.h"
#include "Field.h"
class FOREL
{
private:
	double R;
public:
	FOREL();
	FOREL(const FOREL& f);
	~FOREL() = default;
	const FOREL& operator=(const FOREL& f);
	void assign_R(double r);
	double get_R();
	Find_cluster find_clusters(Field *field);
};

