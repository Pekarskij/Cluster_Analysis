#include "delaunay.h"

void delaunay::delaunay_(vector <Point> points) {
   int n = points.size();
        //sortiruem tochki po x koordinate
   int min_x = 0;
   int iter = 0;
   vector<vector<int>>triangles;
   for (int i = 0; i < n; i++)
   {
       if (points[i].GetX() < points[min_x].GetX())
       {
           min_x = i;
       }
   }
        vector<vector<bool>> edge(n, vector<bool>(n, false));
        Edge = edge;
        //ishchem rebro minimal'noj vypukloj obolochki
        //pervaya vershina - samaya levaya tochka
        //vtoruyu ishchem tak, chtoby vse ostal'nye tochki byli po odnu iz storon ot rebra

        int min_i = 0;
        double min_angle = 90;
        for (int i = 0; i < n; ++i) {
            if (i != min_x) {
                Point temp = Point(points[min_x].GetX(),
                    points[i].GetY());
                vector <Point> temp_edge = { temp, points[i] };
                double curr_angle = delaunay_angle_to_edge(temp_edge, points[min_x]);
                if (min_angle > curr_angle) {
                    min_angle = curr_angle;
                    min_i = i;
                }
            }
        }

        //najdennoe rebro - pervoe bazovoe rebro
        vector<vector <int>> baselines;
        vector <int> edge_ = { min_i, min_x };
        baselines.push_back(edge_);
       
        Edge[min_x][min_i] = true;
        Edge[min_i][min_x] = true;//dobavlyaem rebro v triangulyaciyu

        //ishchem iz ostal'nyh tochek takuyu, chto ona smotrit na rebro pod naibol'shim uglom
        int max_i = 0;
        double max_angle = 0;
        for (int i = 0; i < n; ++i)
        {
            Point vec_1(points[baselines[0][1]].GetX() - points[i].GetX(), points[baselines[0][1]].GetY() - points[i].GetY());
            Point vec_2(points[baselines[0][0]].GetX() - points[i].GetX(), points[baselines[0][0]].GetY() - points[i].GetY());
            if (!((vec_1.length() < 0.000001) || (vec_2.length() < 0.000001))) {
                vector <Point> edge = { points[baselines[0][0]], points[baselines[0][1]] };
                double curr_angle = delaunay_angle_to_edge(edge, points[i]);
                if (max_angle < curr_angle) {
                    max_angle = curr_angle;
                    max_i = i;
                }
               

            }
        }

        //dobavlyaem dva novyh bazovyh rebra
        Edge[baselines[0][0]][max_i] = true;
        Edge[max_i][baselines[0][0]] = true;
         edge_ = { baselines[0][0], max_i };
        baselines.push_back(edge_);
        Edge[baselines[0][1]][max_i] = true;
        Edge[max_i][baselines[0][1]] = true;
        edge_ = { max_i, baselines[0][1] };
        baselines.push_back(edge_);

        vector<int> triangle;
        triangles.push_back(triangle);
        triangles[0].push_back(max_i);
        triangles[0].push_back(baselines[0][1]);
        triangles[0].push_back(baselines[0][0]);
        triangle.clear();

        baselines.erase(baselines.cbegin());//staroe bazovoe rebro ubiraem

        delaunay_base_run(baselines, points,  triangles, iter);//povtoryaem dlya sleduyushchego bazovogo rebra
       // delaunay_fprintf(points);
        triangles.clear();
        baselines.clear();
        edge_.clear();
        gif(iter - 1);
    
}

void delaunay::delaunay_base_run(vector<vector <int>>& baselines, vector <Point> points, vector<vector<int>>& triangles, int &iter) {

    double k, b;
    int max_i=0;
    int p, m = 0;
    bool started = false;
    double max_angle = 0;

    int n = triangles.size();
    //rassmotrim ocherednoe bazovoe rebro
    for (int i = 0; i < n; ++i)//proveryaem skol'ko granej primykaet k rebru(ih ne bol'she dvuh)
    {
        if ( (((triangles[i][0] == baselines[0][0]) && (triangles[i][1] == baselines[0][1]))) || ((triangles[i][0] == baselines[0][1]) && (triangles[i][1] == baselines[0][0])))
        {
            p = triangles[i][2];//zapominaem tret'yu vershinu grani
            m++;
        }
        if ((((triangles[i][0] == baselines[0][0]) && (triangles[i][2] == baselines[0][1]))) || ((triangles[i][0] == baselines[0][1]) && (triangles[i][2] == baselines[0][0])))
        {
            p = triangles[i][1];//zapominaem tret'yu vershinu grani
            m++;
        }
        if ((((triangles[i][1] == baselines[0][0]) && (triangles[i][2] == baselines[0][1]))) || ((triangles[i][1] == baselines[0][1]) && (triangles[i][2] == baselines[0][0])))
        {
            p = triangles[i][0];//zapominaem tret'yu vershinu grani
            m++;
        }
        if (m > 1)
        {
            break;
        }
    }

    if (m == 1)//esli u rebra uzhe est' dve grani, to s nim bol'she nichego ne delaem
    {
        //k, b - koefficienty uravneniya pryamoj y = k*x + b, na kotoroj lezhit rebro
        k = (points[baselines[0][0]].GetY() - points[baselines[0][1]].GetY()) / (points[baselines[0][0]].GetX() - points[baselines[0][1]].GetX());
       // b = (points[baselines[0][1]].GetY() * points[baselines[0][0]].GetX() - points[baselines[0][0]].GetY() * points[baselines[0][1]].GetX()) / (points[baselines[0][0]].GetX() - points[baselines[0][1]].GetX());
        vector <Point> edge = { points[baselines[0][0]], points[baselines[0][1]] };
        b = points[baselines[0][0]].GetY() - k * points[baselines[0][0]].GetX();
        //ishchem tochku, kotoraya smotrit na rebro pod naibol'shim uglom
        int q = points.size();
        for (int i = 0; i < q; ++i) {
            
            Point vec_1(points[baselines[0][1]].GetX() - points[i].GetX(), points[baselines[0][1]].GetY() - points[i].GetY());
            Point vec_2(points[baselines[0][0]].GetX() - points[i].GetX(), points[baselines[0][0]].GetY() - points[i].GetY());
            if (!( (vec_1.length()<0.00001) || (vec_2.length() < 0.00001) )) {

                //novaya tochka i tret'ya tochka grani bazovogo rebra dolzhny byt' po raznye storony
                if ((points[p].GetY() - k * points[p].GetX() - b) * (points[i].GetY() - k * points[i].GetX() - b) < 0)
                {
                    started = true;
                    double curr_angle = delaunay_angle_to_edge(edge, points[i]);
                    if (max_angle < curr_angle) {
                        max_angle = curr_angle;
                        max_i = i;
                    }
                   
                }
            }
        }
        edge.clear();
    }
    //esli nashli nuzhnuyu tochku, to dobavlyaem novye rebra v triangulyaciyu
    //novye ryobra dobavlyaem v bazovye ryobra
    if (started) {
       // if (!(Edge[baselines[0][0]][max_i] && Edge[baselines[0][1]][max_i]))
       // {
            vector<int> triangle;
            triangles.push_back(triangle);
            triangles[n].push_back(max_i);
            triangles[n].push_back(baselines[0][1]);
            triangles[n].push_back(baselines[0][0]);
        //}
        if (!Edge[baselines[0][0]][max_i]) {
            Edge[baselines[0][0]][max_i] = true;
            Edge[max_i][baselines[0][0]] = true;
            vector <int> edge_ = { baselines[0][0], max_i };
            baselines.push_back(edge_);
            edge_.clear();
        }
        if (!Edge[baselines[0][1]][max_i]) {
            Edge[baselines[0][1]][max_i] = true;
            Edge[max_i][baselines[0][1]] = true;
            vector <int> edge_ = { max_i, baselines[0][1] };
            baselines.push_back(edge_);
            edge_.clear();
        }
        triangle.clear();
    }
    baselines.erase(baselines.cbegin());//staroe bazovoe rebro ubiraem
    delaunay_fprintf_iter(iter, points);
    iter++;
    if (!baselines.empty()) {//poka est' bazovye ryobra prodolzhaem algoritm
        delaunay_base_run(baselines, points,  triangles, iter);
    }
}

double delaunay::delaunay_angle_to_edge(vector <Point>& edge, Point c) {
    Point vec_1(edge[0].GetX() - c.GetX(), edge[0].GetY() - c.GetY());
    Point vec_2(edge[1].GetX() - c.GetX(), edge[1].GetY() - c.GetY());
    double dist_a = (c - edge[0]).length();
    double dist_b = (c - edge[1]).length();
    double sc_product = vec_1.GetX() * vec_2.GetX() + vec_1.GetY() * vec_2.GetY();
    double angle = 180 * acos(sc_product / dist_a / dist_b) / (3.1415);
    if (isnan(angle)) {
        return 90;
    }
    return angle;
}

void delaunay::delaunay_fprintf(vector <Point>& points) {
    // prints algorithm's state
    ofstream edges("Algorithms//delaunay//dl.txt"), g("Algorithms//delaunay//dl.plt");
    int q = points.size();
    for (int i = 0; i < q; ++i) {
        for (int j = i + 1; j < q; ++j) {
            if (Edge[i][j]) {
                edges <<points[i].GetX() << " " <<points[i].GetY() << endl;
                edges << points[j].GetX() << " " << points[j].GetY() << endl << endl;
            }
        }
    }
    g << "set size square\n" << "set palette\n" << "plot 'dl.txt' with lines\n";
    g.close();
    edges.close();
}
vector<vector<bool>> delaunay::get_edge()
{
    return Edge;
}
void delaunay::delaunay_fprintf_iter(int iteration, vector <Point>& points) {
    ofstream edges("Algorithms//delaunay//dl_" + to_string(iteration) + ".txt");
   // ofstream g("Algorithms//delaunay//dl" + to_string(iteration)+ ".plt");
    int q = points.size();
    for (int i = 0; i < q; ++i) {
        for (int j = i + 1; j < q; ++j) {
            if (Edge[i][j]) {
                edges << points[i].GetX() << " " << points[i].GetY() << endl;
                edges << points[j].GetX() << " " << points[j].GetY() << endl << endl;
            }
        }
    }
    //g << "set size square\n" << "set palette\n" << "plot " << "'dl" + to_string(iteration) + ".txt'" <<" with lines\n";
}
void delaunay::gif(int iter)
{
    string Animate = "Algorithms//delaunay//delaunay_Animate.plt";
    ofstream Anim;
    Anim.open(Animate);
    Anim << "set xrange[-15:20] " << endl;
    Anim << "set yrange[-15:20] " << endl;
    Anim << "set term gif animate optimize delay 10 background \"#ffeedf\" font \" Times-Roman,10 \" " << endl;
    Anim << "set output \"delaunay_algoritm.gif\" " << endl;
    Anim << "set size square" << endl;
    Anim << "set palette" << endl;
    Anim << "do for[i=0:" << iter << "]{" << endl;
    Anim << "plot ";
    Anim << "'dl_'.i.'.txt'  with lines" << endl;
    Anim << "}" << endl;
    Anim.close();
}
