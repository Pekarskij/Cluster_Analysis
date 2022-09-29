#include "Cloud.h"
#include <time.h>
Cloud::Cloud()
{
    mean_value_for_X = mean_value_for_Y = dispersion_for_X = dispersion_for_Y = 0;
    radius = diameter = number_points_in_cloud = cloud_id = 0;
}

Cloud::Cloud(double mean_x, double mean_y, double sigma_x, double sigma_y, int n) //mean_x, mean_y - centr; sigma_x, sigma_y - otklonenie; n - chislo tochek
{
    double M = 10;
    double x, y;
    vector <double> dist;//matrica rasstoyanij
    vector <double> max_dist(n, 0); //max_dist[i] - maksimal'noe rasstoyanie ot i-toj tochki, diameter - naibol'shij element massiva, radius - naimen'shij
    srand(time(NULL));
    mean_value_for_X = mean_x;
    mean_value_for_Y = mean_y;
    dispersion_for_X = sigma_x;
    dispersion_for_Y = sigma_y;
    radius = 0;
    diameter = 0;
    number_points_in_cloud = n;
    cloud_id = 0;
    if (number_points_in_cloud != 0)
    {
        for (int i = 0; i < number_points_in_cloud; ++i) 
        { //sozdayom oblako
            x = 0;
            y = 0;
            for (int j = 0; j < M; ++j) 
            {
                x += (double)rand() / RAND_MAX;
                y += (double)rand() / RAND_MAX;
            }
            x /= M;
            y /= M;
            x -= 0.5;
            y -= 0.5;
            x *= 4;
            y *= 4;
            x *= sigma_x;
            y *= sigma_y;
            x += mean_value_for_X;
            y += mean_value_for_Y;
            points_in_cloud.push_back(Point(x, y));
        }
    }
    for (int i = 0; i < number_points_in_cloud; i++) points_in_cloud[i].assign_Point_id(i); //numeruem tochki

    //ishchem radius i diametr
    for (int i = 0; i < number_points_in_cloud; i++)
    {
        for (int j = 0; j < number_points_in_cloud; j++)
            dist.push_back((points_in_cloud[i] - points_in_cloud[j]).length());
    }

    for (int i = 0; i < number_points_in_cloud; i++) 
    {
        for (int j = 0; j < number_points_in_cloud; j++)
            if (dist[i * number_points_in_cloud + j] > max_dist[i]) max_dist[i] = dist[i * number_points_in_cloud + j];
    }
    for (int i = 0; i < number_points_in_cloud; i++)
        if (max_dist[i] > diameter) diameter = max_dist[i];
    radius = diameter;
    for (int i = 0; i < number_points_in_cloud; i++)
        if (max_dist[i] < radius) radius = max_dist[i];
    dist.clear();
    max_dist.clear();
}

Cloud::Cloud(const Cloud& c)
{
    mean_value_for_X = c.mean_value_for_X;
    mean_value_for_Y = c.mean_value_for_Y;
    dispersion_for_X = c.dispersion_for_X;
    dispersion_for_Y = c.dispersion_for_Y;
    radius = c.radius;
    diameter = c.diameter;
    number_points_in_cloud = c.number_points_in_cloud;
    cloud_id = c.cloud_id;
    points_in_cloud = c.points_in_cloud;
}

const Cloud& Cloud::operator=(const Cloud& c)
{
    mean_value_for_X = c.mean_value_for_X;
    mean_value_for_Y = c.mean_value_for_Y;
    dispersion_for_X = c.dispersion_for_X;
    dispersion_for_Y = c.dispersion_for_Y;
    radius = c.radius;
    diameter = c.diameter;
    number_points_in_cloud = c.number_points_in_cloud;
    cloud_id = c.cloud_id;
    points_in_cloud = c.points_in_cloud;
    return *this;
}

Point Cloud::get_cloud_center() { return Point(mean_value_for_X, mean_value_for_Y); }

Point Cloud::get_dispersions() { return Point(dispersion_for_X, dispersion_for_Y); }

double Cloud::get_radius() { return radius; }

double Cloud::get_diameter() { return diameter; }

int Cloud::get_number_points_in_cloud() { return number_points_in_cloud; }

int Cloud::get_cloud_id() { return cloud_id; }

Point Cloud::get_point_in_cloud(int point_id) { return points_in_cloud[point_id]; }

void Cloud::assign_cloud_id(int id) { cloud_id = id; }

void Cloud::assign_point_id_in_cloud(int n, int id) { points_in_cloud[n].assign_Point_id(id); }

void Cloud::print_cloud(ofstream& file)
{
    for (int i = 0; i < number_points_in_cloud; i++)
        file << points_in_cloud[i].GetX() << " " << points_in_cloud[i].GetY() << " " << cloud_id << endl;
}

void Cloud::creat_cloud_histogram(int number_of_columns)
{
    ofstream f_x, f_y, g_x, g_y;
    int i, j;
    double max_x, min_x, d_x, max_y, min_y, d_y;
    vector <int> hist_x(number_of_columns, 0);
    vector <int> hist_y(number_of_columns, 0);
    Point v = get_point_in_cloud(0);
    string s_1 = to_string(cloud_id), 
        s_2("Field and histograms//"), 
        s_3("Field and histograms//"),
        s_4("Field and histograms//"),
        s_5("Field and histograms//");
    s_2.append(s_1);
    s_2.append("-th cloud histogram for first coordinate.txt");
    s_3.append(s_1);
    s_3.append("-th cloud histogram for first coordinate.plt");
    s_4.append(s_1);
    s_4.append("-th cloud histogram for second coordinate.txt");
    s_5.append(s_1);
    s_5.append("-th cloud histogram for second coordinate.plt");
    
    f_x.open(s_2);
    g_x.open(s_3);
    g_x << "set style data histogram\n" << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale x\n" << "set autoscale y\n" << "set boxwidth 2\n" 
        << "plot '"<< s_1 << "-th cloud histogram for first coordinate.txt'\n";
    f_y.open(s_4);
    g_y.open(s_5);
    g_y << "set style data histogram\n" << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale x\n" << "set autoscale y\n" << "set boxwidth 2\n" 
        << "plot '" << s_1 << "-th cloud histogram for second coordinate.txt'\n";

    min_x = v.GetX();
    max_x = v.GetX();
    min_y = v.GetY();
    max_y = v.GetY();

    for (i = 0; i < number_points_in_cloud; i++)
    {
        v = get_point_in_cloud(i);
        if (v.GetX() < min_x) min_x = v.GetX();
        if (v.GetX() > max_x) max_x = v.GetX();
        if (v.GetY() < min_y) min_y = v.GetY();
        if (v.GetY() > max_y) max_y = v.GetY();
    }
    d_x = (max_x - min_x) / number_of_columns;
    d_y = (max_y - min_y) / number_of_columns;
    for (i = 0; i < number_points_in_cloud; i++)
    {
        v = get_point_in_cloud(i);
        for (j = 1; j < number_of_columns + 1; j++)
        {
            if (v.GetX() < (min_x + j * d_x) && v.GetX() >= (min_x + (j - 1) * d_x)) hist_x[j - 1]++;
            if (v.GetY() < (min_y + j * d_y) && v.GetY() >= (min_y + (j - 1) * d_y)) hist_y[j - 1]++;
        }
    }
    for (i = 0; i < number_of_columns; i++)
    {
        f_x << hist_x[i] << endl;
        f_y << hist_y[i] << endl;
    }
    f_x.close();
    g_x.close();
    f_y.close();
    g_y.close();
    hist_x.clear();
    hist_y.clear();
}

void Cloud::shift_cloud(Point v)
{
    mean_value_for_X = get_cloud_center().GetX() + v.GetX();
    mean_value_for_Y = get_cloud_center().GetY() + v.GetY();
    for (int i = 0; i < number_points_in_cloud; i++) points_in_cloud[i] = points_in_cloud[i] + v;
}

void Cloud::turn_cloud(double phi)
{
    double x, y;

    for (int i = 0; i < number_points_in_cloud; i++)
    {
        x = cos(phi* 3.1415/180) * (points_in_cloud[i].GetX() - mean_value_for_X) - sin(phi * 3.1415 / 180) * (points_in_cloud[i].GetY() - mean_value_for_Y);
        y = sin(phi * 3.1415 / 180) * (points_in_cloud[i].GetX() - mean_value_for_X) - cos(phi * 3.1415 / 180) * (points_in_cloud[i].GetY() - mean_value_for_Y);
        points_in_cloud[i] = Point(x, y) + get_cloud_center();
    }
}

void Cloud::compression_cloud(double lambda)
{
    for (int i = 0; i < number_points_in_cloud; i++)
    {
        points_in_cloud[i] = lambda * (points_in_cloud[i] - get_cloud_center());
        points_in_cloud[i] = points_in_cloud[i] + get_cloud_center();
    }
}