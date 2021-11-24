#include "Field.h"

Field::Field() { number_clouds_in_field = number_points_in_field = 0; }

Field::Field(const Field& f)
{
    number_clouds_in_field = f.number_clouds_in_field;
    number_points_in_field = f.number_points_in_field;
    clouds_in_field = f.clouds_in_field;
    buffer = f.buffer;
}

const Field& Field::operator=(const Field& f)
{
    number_clouds_in_field = f.number_clouds_in_field;
    number_points_in_field = f.number_points_in_field;
    clouds_in_field = f.clouds_in_field;
    buffer = f.buffer;
    return *this;
}

void Field::add_cloud_in_field(Cloud c)
{
    int i, k = number_points_in_field;

    number_clouds_in_field++;
    number_points_in_field = number_points_in_field + c.get_number_points_in_cloud();
    clouds_in_field.push_back(c);
    clouds_in_field[number_clouds_in_field - 1].assign_cloud_id(number_clouds_in_field - 1);
    for (i = 0; i < clouds_in_field[number_clouds_in_field - 1].get_number_points_in_cloud(); i++)
    {
        clouds_in_field[number_clouds_in_field - 1].assign_point_id_in_cloud(i, k);
        k++;
    }
}

void Field::delete_cloud_from_field(int id)
{
    int i, j, k = 0;

    number_points_in_field = number_points_in_field - clouds_in_field[id].get_number_points_in_cloud();
    number_clouds_in_field--;
    clouds_in_field.erase(clouds_in_field.begin() + id);
    for (i = 0; i < number_clouds_in_field; i++) clouds_in_field[i].assign_cloud_id(i);
    for (i = 0; i < number_clouds_in_field; i++)
    {
        for (j = 0; j < clouds_in_field[i].get_number_points_in_cloud(); j++)
        {
            clouds_in_field[i].assign_point_id_in_cloud(j, k);
            k++;
        }
    }
}

void Field::print_field()
{
    ofstream f("Field and histograms//Field.txt"),
        g("Field and histograms//Field.plt");

    for (int i = 0; i < number_clouds_in_field; i++) clouds_in_field[i].print_cloud(f);
    g << "set size square\n" << "set palette\n" << "plot 'Field.txt' palette\n";
    f.close();
    g.close();
}

vector <Point> Field::get_points_from_field()
{
    vector <Point> points;

    for (int i = 0; i < number_clouds_in_field; i++)
    {
        for (int j = 0; j < clouds_in_field[i].get_number_points_in_cloud(); j++) points.push_back(clouds_in_field[i].get_point_in_cloud(j));
    }
    return points;
}

void Field::add_cloud_from_field_in_buffer(int id)
{
    buffer.add_cloud_in_buffer(clouds_in_field[id]);
    delete_cloud_from_field(id);
}

void Field::add_all_clouds_from_field_in_buffer()
{
    for (int i = 0; i < number_clouds_in_field; i++)
    {
        buffer.add_cloud_in_buffer(clouds_in_field[i]);
        delete_cloud_from_field(i);
    }
}

void Field::upload_cloud_from_buffer_in_field(int n) { add_cloud_in_field(buffer.upload_cloud_from_buffer(n)); }

void Field::upload_all_clouds_from_buffer_in_field()
{
    for (int i = 0; i < buffer.get_number_clouds_in_buffer(); i++) add_cloud_in_field(buffer.upload_cloud_from_buffer(i));
}

void Field::shift_cloud_in_buffer(int n, Point v) { buffer.shift_cloud_in_buffer(n, v); }

void Field::shift_all_clouds_in_buffer(Point v) { buffer.shift_all_clouds_in_buffer(v); }

void Field::turn_cloud_in_bufffer(int n, double phi) { buffer.turn_cloud_in_buffer(n, phi); }

void Field::turn_all_clouds_in_bufffer(double phi) { buffer.turn_all_clouds_in_buffer(phi); }

void Field::compression_cloud_in_buffer(int n, double lambda) { buffer.compression_cloud_in_buffer(n, lambda); }

void Field::compression_all_clouds_in_buffer(double lambda) { buffer.compression_all_clouds_in_buffer(lambda); }

void Field::creat_field_histogram(int number_of_columns)
{
    ofstream f_x("Field and histograms//Field histogram for first coordinate.txt"),
        f_y("Field and histograms//Field histogram for second coordinate.txt");
    ofstream g_x("Field and histograms//Field histogram for first coordinate.plt"),
        g_y("Field and histograms//Field histogram for second coordinate.plt");
    double min_x, max_x, min_y, max_y, d_x, d_y;
    int i, j, k;
    vector <int> hist_x(number_of_columns, 0);
    vector <int> hist_y(number_of_columns, 0);
    Point v = clouds_in_field[0].get_point_in_cloud(0);

    g_x << "set style data histogram\n" << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale x\n" << "set autoscale y\n" << "set boxwidth 2\n" << "plot 'Field histogram for first coordinate.txt'\n";
    g_y << "set style data histogram\n" << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale x\n" << "set autoscale y\n" << "set boxwidth 2\n" << "plot 'Field histogram for second coordinate.txt'\n";

    min_x = v.GetX();
    max_x = v.GetX();
    min_y = v.GetY();
    max_y = v.GetY();

    for (i = 0; i < number_clouds_in_field; i++)
    {
        for (j = 0; j < clouds_in_field[i].get_number_points_in_cloud(); j++)
        {
            v = clouds_in_field[i].get_point_in_cloud(j);
            if (v.GetX() < min_x) min_x = v.GetX();
            if (v.GetX() > max_x) max_x = v.GetX();
            if (v.GetY() < min_y) min_y = v.GetY();
            if (v.GetY() > max_y) max_y = v.GetY();
        }
    }
    d_x = (max_x - min_x) / number_of_columns;
    d_y = (max_y - min_y) / number_of_columns;
    for (i = 0; i < number_clouds_in_field; i++)
    {
        for (j = 0; j < clouds_in_field[i].get_number_points_in_cloud(); j++)
        {
            v = clouds_in_field[i].get_point_in_cloud(j);
            for (k = 1; k < number_of_columns + 1; k++)
            {
                if (v.GetX() < (min_x + k * d_x) && v.GetX() >= (min_x + (k - 1) * d_x)) hist_x[k - 1]++;
                if (v.GetY() < (min_y + k * d_y) && v.GetY() >= (min_y + (k - 1) * d_y)) hist_y[k - 1]++;
            }
        }
    }
    for (i = 0; i < number_of_columns; i++)
    {
        f_x << hist_x[i] << endl;
        f_y << hist_y[i] << endl;
    }
    f_x.close();
    f_y.close();
    g_x.close();
    g_y.close();
}

void Field::creat_cloud_histogram(int id, int number_of_colunms) { clouds_in_field[id].creat_cloud_histogram(number_of_colunms); }
