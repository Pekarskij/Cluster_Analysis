#include "Spanning_tree.h"

vector <pair <double, pair <int, int>>> Spanning_tree::creat_spanning_tree(vector <Point>& points)
{
    int i, j, i_1, j_1, k, n = points.size();
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
    for (i = 0; i < n; i++)
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
    tree[i_1]++;
    tree[j_1]++;
    while (k != n)
    {
        d = 10000;
        for (i = 0; i < n; i++)
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

void Spanning_tree::print_spanning_tree(vector <Point>& points, vector <pair <double, pair <int, int>>>& edges)
{
    ofstream f("Algorithms//stree//Spanning tree (graph).txt"), g("Algorithms//stree//Spanning tree (graph).plt");

    g << "set nokey\n" << "set size square\n" << "plot 'Spanning tree (graph).txt' with linespoints title 'Spanning tree'\n";
    for (int i = 0; i < points.size() - 1; i++)
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
    g << "set style fill solid\n" << "set yrange [0:]\n" << "set autoscale y\n" << "set boxwidth 1\n" << "plot 'Spanning tree (histogram).txt' with boxes\n";
    for (int i = 0; i < points.size() - 1; i++)
    {
        f << edges[i].first << " " << i << "\n";
    }
    f.close();
    g.close();
}
