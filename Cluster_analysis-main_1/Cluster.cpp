#include "Cluster.h"

Cluster::Cluster() { number_points_in_cluster = cluster_id = radius = diameter = 0; cluster_center = Point(0, 0); }

Cluster::Cluster(const Cluster& c)
{
    number_points_in_cluster = c.number_points_in_cluster;
    cluster_id = c.cluster_id;
    radius = c.radius;
    diameter = c.diameter;
    cluster_center = c.cluster_center;
    point_indicators = c.point_indicators;
}

const Cluster& Cluster::operator=(const Cluster& c)
{
    number_points_in_cluster = c.number_points_in_cluster;
    cluster_id = c.cluster_id;
    radius = c.radius;
    diameter = c.diameter;
    cluster_center = c.cluster_center;
    point_indicators = c.point_indicators;
    return *this;
}

int Cluster::get_number_points_in_cluster() { return number_points_in_cluster; }

int Cluster::get_cluster_id() { return cluster_id; }

int Cluster::get_point_indicator(int id) { return point_indicators[id]; }

Point Cluster::get_cluster_center() { return cluster_center; }

double Cluster::get_radius() { return radius; }

double Cluster::get_diameter() { return diameter; }

void Cluster::assign_cluster_id(int id) { cluster_id = id; }

void Cluster::add_point_indicator(int n)//n = 1 ili 0; point_indicators[i]=1 - i-tay tochka polya v clustere, point_indicators[i]=0 - i-tay tochka polya ne v clustere
{
    if (n == 1) number_points_in_cluster++;
    point_indicators.push_back(n);
}


void Cluster::assign_point_indicator(int id, int n)  //point_indicators[i]=1 - i-tay tochka polya v clustere, point_indicators[i]=0 - i-tay tochka polya ne v clustere
{ 
    if (point_indicators[id] == 1 && n == 0) number_points_in_cluster--; //esli tochka byla v klastere i n=0, to udalyaem tochku
    if (point_indicators[id] == 0 && n == 1) number_points_in_cluster++;//esli tochki ne bylo v klastere i n=1, to dobavlyaem tochku
    point_indicators[id] = n;
}

void Cluster::assign_cluster_center(Point p) { cluster_center = p; }

    void Cluster::print_cluster(ofstream& file, Field * field)
    {
        vector<Point> points = field->get_points_from_field();
        int n = point_indicators.size();
        for (int i = 0; i < n; i++)
            if (point_indicators[i] == 1) file << points[i].GetX() << " " << points[i].GetY() << " " << cluster_id << "\n";
        points.clear();
    }

    void Cluster::find_cluster_center(vector <Point>& points)
    {
        Point sum(0, 0);
        int N = 0;
    int n = points.size();
        for (int i = 0; i < n; i++)
        {
            if (point_indicators[i] == 1)
            {
                sum = sum + points[i];
                N++;
            }
        }
        sum = (1 / N) * sum;
        cluster_center = sum;
    }
    
    void Cluster::find_radius_and_diameter(vector <Point>& points)
    {
        int i, j, N = 0;
        vector <double> dist;
        vector <double> max_dist(number_points_in_cluster, 0);
        vector <Point> points_in_cluster;
    int n = points.size();
        for (i = 0; i < n; i++)
        {
            if (point_indicators[i] == 1)
            {
                points_in_cluster.push_back(points[i]);
                N++;
            }
        }
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
                dist.push_back((points_in_cluster[i] - points_in_cluster[j]).length());
        }
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
                if (dist[i * N + j] > max_dist[i]) max_dist[i] = dist[i * N + j];
        }
        for (i = 0; i < N; i++)
            if (max_dist[i] > diameter) diameter = max_dist[i];
        radius = diameter;
       for (i = 0; i < N; i++)
            if (max_dist[i] < radius) radius = max_dist[i];
       dist.clear();
       max_dist.clear();
       points_in_cluster.clear();
    }
