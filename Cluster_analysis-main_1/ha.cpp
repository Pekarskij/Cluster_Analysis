#include "ha.h"

ha::ha() { k = 0; t = 0; }

ha::ha(const ha& h) { k = h.k; t = h.t; }

const ha& ha::operator=(const ha& h) { k = h.k; t = h.t; return *this; }

void ha::assing_k(int k_k) { k = k_k; }

void ha::assing_t(int t_t) { t = t_t; }

double ha::distance_for_hierarchy(vector <int>& a, vector <int>& b, vector <Point>& points)
{
    int n = a.size(), m = b.size(), i, j;
    double dist = 0;
    double temp;

    if (t == 1)//Metod srednej svyazi
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++) dist = dist + (points[a[i]] - points[b[j]]).length();
        }
        dist = dist / (n * m);
    }
    else if (t == 2)//Metod odinochnoj svyazi
    {
        dist = (points[a[0]] - points[b[0]]).length();
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                temp = (points[a[i]] - points[b[j]]).length();
                if (temp < dist)
                {
                    dist = temp;
                }
            }
        }
    }
    else//Metod polnoj svyazi
    {
        dist = (points[a[0]] - points[b[0]]).length();
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                temp = (points[a[i]] - points[b[j]]).length();
                if (temp > dist)
                {
                    dist = temp;
                }
            }
        }
    }

    return dist;
}

vector <int> ha::tree_walk(vector <int>& T, int n, vector <vector <int>>& point_indicators)
{
    int i_1, i_2;
    i_1 = point_indicators[n][0];
    i_2 = point_indicators[n][1];
    if ((i_1 == 0) && (i_2 == 0))//spuskaemsya po derevu poka ne dojdyom do edinichnogo klastera
    {
        T.push_back(n);
    }
    else
    {
        tree_walk(T, i_1, point_indicators);
        tree_walk(T, i_2, point_indicators);
    }
    return T;
}

Find_cluster ha::hierarchy(Field *field)
{
    ofstream f("Algorithms//ha//Hierarchial algorithm (graph).txt"), g("Algorithms//ha//Hierarchial algorithm (graph).plt");
    vector<Point> points_from_field = field->get_points_from_field();
    int n = points_from_field.size(), l = n, i, j, N = n, i_1, i_2, m;
    vector <double> Dist((2 * n - 1) * (2 * n - 1), 0);
    double min_D;
    Point v;
    vector <Point> points;
    vector <int> a;
    vector <int> b;
    vector <vector <int>> point_indicators(2 * n);
    vector <Cluster> P(k);
    Find_cluster fc;

    for (i = 0; i < n; i++)
    {
      
        points.push_back(points_from_field[i]);
        point_indicators[i].push_back(0); //point_indicators[i][0] - nomer levogo rebyonka
        point_indicators[i].push_back(0); //point_indicators[i][1] - nomer pravogo rebyonka
        //point_indicators[i].push_back(0);  //point_indicators[i][2] = k -  i-yj klaster na k-om shage ne slilsya s drugim
        point_indicators[i].push_back(0);   // point_indicators[i][2]= 1/0 - i-yj klaster chej-to rebyonok / i-yj klaster nichej rebyonok
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++) Dist[i * (2 * n - 1) + j] = (points[i] - points[j]).length();
    }


    while (l > 1)
    {
        if (l == k) {

            for (i = 0; i < k; i++)
            {
                for (j = 0; j < n; j++) P[i].add_point_indicator(0);
            }
            m = 0;
            for (i = 0; i < N; i++)
            {
                if (point_indicators[i][2] == 0)
                {
                    a = tree_walk(a, i, point_indicators);
                    int q = a.size();
                    for (int s = 0; s < q; s++)
                    {
                        P[m].assign_point_indicator(a[s], 1);
                    }
                    a.clear();

                    m++;
                }
                if (m == k)
                {
                    break;
                }
            }
        }

        min_D = 10000;
        for (i = 0; i < N; i++)//ishchem blizhajshie klastery
        {
            if (point_indicators[i][2] == 0)
            {
                for (j = 0; j < N; j++)
                {
                    if (Dist[i * (2 * n - 1) + j] < min_D && i != j && point_indicators[j][3] == 0)
                    {
                        i_1 = i;
                        i_2 = j;
                        min_D = Dist[i * (2 * n - 1) + j];
                    }
                }
            }
        }
        
        point_indicators[i_1][2] = 1; //dva klastera slilis' v odin i my i bol'she ne rassmatrivaem
        point_indicators[i_2][2] = 1;
        N++;//dobavilsya novyj klaster
        v = 0.5 * (points[i_1] + points[i_2]);
        points.push_back(v);
        point_indicators[N - 1].push_back(i_1);//deti novogo klastera - dva klastera, iz kotoryh on poluchilsya
        point_indicators[N - 1].push_back(i_2);
       // point_indicators[N - 1].push_back(N - n);
        point_indicators[N - 1].push_back(0);
        //for (i = 0; i < N - 1; i++)
       // {
        //    if (point_indicators[i][3] == 0) point_indicators[i][2] == N - n;
        //}
        a = tree_walk(a, N - 1, point_indicators);
       
        for (i = 0; i < N - 1; i++)//schitaem rasstoyaniya ot novogo klastera do vsekh ostal'nyh
        {
            if (point_indicators[i][2] == 0)
            {
                b = tree_walk(b, i, point_indicators);
                Dist[(N - 1) * (2 * n - 1) + i] = distance_for_hierarchy(a, b, points);
                Dist[i * (2 * n - 1) + N - 1] = Dist[(N - 1) * (2 * n - 1) + i];
                b.clear();
            }
        }
       
        a.clear();
        l--;
    }

    print_tree(f, points, N - 1, point_indicators);
    g << "set size square\n" << "plot 'Hierarchial algorithm (graph).txt' with linespoints title 'Hierarchial algorithm'\n";
    f.close();
    g.close();
    for (i = 0; i < k; i++)
    {
        fc.add_cluster(P[i]);
    }
    return fc;
}

void ha::print_tree(ofstream& file, vector <Point>& points, int n, vector <vector <int>>& point_indicators)
{
    int i_1, i_2;

    i_1 = point_indicators[n][0];
    i_2 = point_indicators[n][1];
    if (i_1 != 0 || i_2 != 0)
    {
        points[n].print_Point(file);
        points[i_1].print_Point(file);
        file << "\n\n";
        points[n].print_Point(file);
        points[i_2].print_Point(file);
        file << "\n\n";
        print_tree(file, points, i_1, point_indicators);
        print_tree(file, points, i_2, point_indicators);

    }
}
