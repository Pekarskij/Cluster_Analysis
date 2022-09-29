#include "kmcores.h"
#include "k_means.h"

kmcores::kmcores() { k = 0; p = 0; }

kmcores::kmcores(const kmcores& km) { k = km.k; p = km.p; }

const kmcores& kmcores::operator=(const kmcores& km) { k = km.k; p = km.p; return *this; }

void kmcores::assing_k(int k_k) { k = k_k; }
void kmcores::assing_p(int p_p) { p = p_p; }

Find_cluster kmcores::find_clusters(Field *field, vector<vector<Point>>& kmcores)
{
    vector<Point> points = field->get_points_from_field();
    double dist;
    int i, j, m, s, c, N = field->get_number_points_in_field();
    vector <Cluster> P(k);
    Find_cluster f;
    vector <Point> k_means(p, Point(0,0));
    vector<Point> cent_cores(k);
    vector <int> n(p, 0);
    vector <double> sum_x(p, 0);
    vector <double> sum_y(p, 0);
    vector <Cluster> T(p);
    int iter = 0;
    kmcores.resize(k);
    for (i = 0; i < k; i++)
    {
        kmcores[i].resize(p);
    }


    for (i = 0; i < k; i++)
    {
        for (j = 0; j < N; j++) P[i].add_point_indicator(0);
    }
    for (i = 0; i < k; i++)//pervichnye centry
    {
        kmcores[i][0] = points[i];
        P[i].assign_point_indicator(i, 1);
        P[i].assign_cluster_id(i);
    }
    for (i = 0; i < N; i++)
    {
        m = 0;
        dist = (points[i] - kmcores[0][0]).length();
        for (j = 0; j < k; j++)
        {
            if ((points[i] - kmcores[j][0]).length() < dist)
            {
                dist = (points[i] - kmcores[j][0]).length();
                m = j;
            }
        }
        P[m].assign_point_indicator(i, 1); //i-tay tochka poluchaet marker m
    }
    for (i = 0; i < p; i++)
    {
        for (j = 0; j < N; j++)
        {

            T[i].add_point_indicator(0);
        }
    }




    c = 1;
    while (c != 0)
    {

        //primenyaem kmeans s parametrom p dlya kazhdoj gruppy
        c = 0;
        for (int t = 0; t < k; t++)
        {
            for (i = 0; i < p; i++)
            {
                n[i] = 0;
                sum_x[i] = 0;
                sum_y[i] = 0;
            }
             s = 1;
            for (i = 0; i < p; i++)
            {
                for (j = 0; j < N; j++)
                {
                    
                        T[i].assign_point_indicator(j, 0);
                }
            }


            int l = 0;

                for (j = 0; j < N; j++)
                {
                    if (P[t].get_point_indicator(j) == 1)
                    {
                        k_means[l] = points[j];
                        T[l].assign_point_indicator(j, 1);
                        l++;

                    }
                    if (l == p)
                    {
                        break;
                    }
                }
            
            for (i = 0; i < N; i++)
            {
                if (P[t].get_point_indicator(i) == 1)
                {
                    m = 0;
                    dist = (points[i] - k_means[0]).length();
                    for (j = 0; j < p; j++)
                    {
                        if ((points[i] - k_means[j]).length() < dist)
                        {
                            dist = (points[i] - k_means[j]).length();
                            m = j;
                        }
                    }
                    T[m].assign_point_indicator(i, 1); //i-tay tochka poluchaet marker m
                }
            }
            while (s != 0)
            {
                s = 0;
                for (i = 0; i < N; i++)
                {
                    if (P[t].get_point_indicator(i) == 1) {
                        for (j = 0; j < p; j++)//schitaem centr tyazhesti
                        {
                            sum_x[j] = sum_x[j] + points[i].GetX() * T[j].get_point_indicator(i);
                            sum_y[j] = sum_y[j] + points[i].GetY() * T[j].get_point_indicator(i);
                            n[j] = n[j] + T[j].get_point_indicator(i);
                        }
                    }
                }
         
                for (i = 0; i < p; i++)
                {
                    sum_x[i] = sum_x[i] / n[i];
                    sum_y[i] = sum_y[i] / n[i];
                    k_means[i] = Point(sum_x[i], sum_y[i]);//novye centry
                    sum_x[i] = 0;
                    sum_y[i] = 0;
                    n[i] = 0;
                }
                for (i = 0; i < N; i++)//proveryaem pomenyala li tochka marker
                {
                    if (P[t].get_point_indicator(i) == 1)
                    {
                        m = 0;
                        dist = (points[i] - k_means[0]).length();
                        for (j = 0; j < p; j++)
                        {
                            if ((points[i] - k_means[j]).length() < dist)
                            {
                                dist = (points[i] - k_means[j]).length();
                                m = j;
                            }
                        }
                        if (T[m].get_point_indicator(i) == 0)//tochka pomenyala marker
                        {
                            for (j = 0; j < p; j++) T[j].assign_point_indicator(i, 0);
                            T[m].assign_point_indicator(i, 1);
                            s++;
                        }
                    }
                    
                }
            }


            for (i = 0; i < p; i++)
            {
                kmcores[t][i] = k_means[i];
            }
           
        }
        for (i = 0; i < k; i++)
        {
            sum_x[i] = 0;
            sum_y[i] = 0;
            for (j = 0; j < p; j++)
            {
                sum_x[i] += kmcores[i][j].GetX();
                sum_y[i] += kmcores[i][j].GetY();
            }
            cent_cores[i] = Point(sum_x[i]/p, sum_y[i]/p);
        }

        for (i = 0; i < N; i++)//proveryaem pomenyala li tochka marker
        {
          
                m = 0;
                dist = (points[i] - cent_cores[0]).length();
                for (j = 0; j < k; j++)
                {
                    if ((points[i] - cent_cores[j]).length() < dist)
                    {
                        dist = (points[i] - cent_cores[j]).length();
                        m = j;
                    }

                }
                if (P[m].get_point_indicator(i) == 0)//tochka pomenyala marker
                {
                    for (j = 0; j < k; j++) P[j].assign_point_indicator(i, 0);
                    P[m].assign_point_indicator(i, 1);
                    c++;
                }
            
        }
        print_iter(P, field, iter);
        iter++;

    }

    for (i = 0; i < k; i++)
    {
        P[i].assign_cluster_center(cent_cores[i]);
        f.add_cluster(P[i]);
    }
    gif(iter - 1);
    k_means.clear();
    P.clear();
    T.clear();
    return f;
}
void kmcores::print_iter(vector <Cluster> P, Field* field, int iter)
{
    string s_1 = to_string(iter),
        s_2("Algorithms//kmcores//kmcores_");
    s_2.append(s_1);
    s_2.append(".txt");
    ofstream f(s_2);
    for (int i = 0; i < k; i++) P[i].print_cluster(f, field);
    f.close();
}
void kmcores::gif(int iter)
{
    string Animate = "Algorithms//kmcores//kmcores_Animate.plt";
    ofstream Anim;
    Anim.open(Animate);
    Anim << "set xrange[-15:20] " << endl;
    Anim << "set yrange[-15:20] " << endl;
    Anim << "set term gif animate optimize delay 10 background \"#ffeedf\" font \" Times-Roman,10 \" " << endl;
    Anim << "set output \"Kmcores_algoritm.gif\" " << endl;
    Anim << "set size square" << endl;
    Anim << "set palette" << endl;
    Anim << "do for[i=0:" << iter << "]{" << endl;
    Anim << "plot ";
    Anim << "'kmcores_'.i.'.txt' palette notitle" << endl;
    Anim << "}" << endl;
    Anim.close();
}
