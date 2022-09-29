#include "Spanning_tree.h"
Spanning_tree::Spanning_tree()
{
    delta = 0;
}
double Spanning_tree::get_delta()
{
    return delta;
}
vector <pair <double, pair <int, int>>> Spanning_tree::creat_spanning_tree(Field *field)
{
    vector<Point> points = field->get_points_from_field();
    int i, j, i_1, j_1, k, n = field->get_number_points_in_field();
    double d;
    vector <int> tree(n, 0);
    vector <pair <double, pair <int, int>>> edges(n - 1);
    vector <double> D;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            D.push_back((points[i] - points[j]).length());
        }
    }

    d = D[1];
    i_1 = 0;
    j_1 = 1;
    for (i = 0; i < n; i++)//ishchem minimal'noe rebro
    {
        for (j = 0; j < n; j++)
        {
            if (D[i*n + j] < d && i != j)
            {
                d = D[i*n + j];
                i_1 = i;
                j_1 = j;
            }
        }
    }
    edges[0].first = d;
    edges[0].second.first = i_1;
    edges[0].second.second = j_1;
    k = 2;
    tree[i_1]++;//dobavlyaem tochki v derevo
    tree[j_1]++;


    while (k != n)
    {
        d = 10000;
        for (i = 0; i < n; i++)//ishchem blizhajshuyu tochku k derevu
        {
            if (tree[i] == 1)
            {
                for (j = 0; j < n; j++)
                {
                    if (D[i*n + j] < d && tree[j] == 0)
                    {
                        d = D[i*n + j];
                        i_1 = i;
                        j_1 = j;
                    }
                }
            }
        }
        edges[k - 1].first = d;
        edges[k - 1].second.first = i_1;
        edges[k - 1].second.second = j_1;
        tree[j_1]++;
        k++;
    }
    return edges;
}

void Spanning_tree::print_spanning_tree(Field *field, vector <pair <double, pair <int, int>>>& edges)
{
    ofstream f("Algorithms//stree//Spanning tree (graph).txt"), g("Algorithms//stree//Spanning tree (graph).plt");

    g << "set nokey\n" << "set size square\n" << "plot 'Spanning tree (graph).txt' with linespoints title 'Spanning tree'\n";
    vector<Point> points = field->get_points_from_field();
    int q = points.size();
    for (int i = 0; i < q - 1; i++)
    {
        int i_1, j_1;
        i_1 = edges[i].second.first;
        j_1 = edges[i].second.second;
        points[i_1].print_Point(f);
        points[j_1].print_Point(f);
        f << "\n";
    }
    f.close();
    g.close();
    f.open("Algorithms//stree//Spanning tree (histogram).txt");
    g.open("Algorithms//stree//Spanning tree (histogram).plt");
    g << "set style data histogram\n" << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale x\n" << "set autoscale y\n" << "set boxwidth 2\n" << "plot 'Spanning tree (histogram).txt' with boxes\n";

    //stroim gistogrammu
    int n = 14;
    double min = edges[0].first, max = edges[0].first, d;
    vector<int> hist(30,0);
    for (int i = 0; i < q - 1; i++)
    {
        if (min > edges[i].first)
        {
            min = edges[i].first;
        }
        if (max < edges[i].first)
        {
            max = edges[i].first;
        }
    }
    d = (max - min) / n;
    hist[n - 1]++;
    for (int i = 0; i < q - 1; i++)
    {
        for (int k = 1; k < n + 1; k++)
        {
            if (edges[i].first < (min + k * d) && edges[i].first >= (min + (k - 1) * d))
            {
                hist[k - 1]++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        f << hist[i] << endl;
    }
    int m = 0;
    for (int i = 0; i < n; i++)
    {
        if (hist[i] < hist[m])
        {
            m = i;
        }
    }
    delta = min + m * d;
    f.close();
    g.close();
}
