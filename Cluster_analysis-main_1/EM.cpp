#include "EM.h"
EM::EM() { k = 0; }

EM::EM(const EM& em) { k = em.k; }

const EM& EM::operator=(const EM& em) { k = em.k; return *this; }

void EM::assign_k(int k_k) { k = k_k; }

Find_cluster EM::find_clusters( Field *field)  {

    vector<vector<double>> sigma;//Matrica kovariacii
    vector<vector<double>> mu; //mu[j][0] - mat ozhidanie pervoj koordinaty j-ogo klastera, mu[j][1] - mat ozhidanie vtoroj koordinaty j-ogo klastera
    vector<vector<double>> g;//g[i][j] -  Veroyatnost' popadaniya i-toj tochki v j-yj klaster
    vector<double> w; //w[j] -  veroyatnost' poyavleniya tochki iz j-ogo klastera
    vector<Point> points = field->get_points_from_field();
    vector<double> v0;
    vector <Cluster> P(k);
    Find_cluster fc;
   int  n = field->get_number_points_in_field();

  // Inicializiruem matricy g, w, mu
    for (int i = 0; i < k; ++i) {
        vector<vector<double>> v22;
        vector<double> v4;
        vector<double> v2;
        v4.reserve (4);
        for (int j = 0; j < 4; j++) {
            v4.push_back (0);
        }
        v2.reserve (2);
        for (int j = 0; j < 2; j++) {
            v2.push_back (0);
        };
        sigma.push_back (v4);
        mu.push_back (v2);
        mu[i][0] = points[i].GetX();
        mu[i][1] = points[i].GetY();
        w.push_back ((double) 1 / k);
        v0.push_back (0);

        P[i].assign_cluster_id(i);
        for (int j = 0; j < n; j++) P[i].add_point_indicator(0);
    
    }
    for (int i = 0; i < n; ++i) {
        g.push_back (v0);
    }
    for (int i = 0; i < k; ++i) {
        vector<vector<double>> a (2, vector<double> (2, 0));
        for (int j = 0; j < n; ++j) {
            a[0][0] += (points[j].GetX() - mu[i][0]) * (points[j].GetX() - mu[i][0]);
            a[0][1] += (points[j].GetX() - mu[i][0]) * (points[j].GetY() - mu[i][1]);
            a[1][0] += (points[j].GetY() - mu[i][1]) * (points[j].GetX() - mu[i][0]);
            a[1][1] += (points[j].GetY() - mu[i][1]) * (points[j].GetY() - mu[i][1]);
        }
        sigma[i][0] = a[0][0] / n;
        sigma[i][1] = a[0][1] / n;
        sigma[i][2] = a[1][0] / n;
        sigma[i][3] = a[1][1] / n;
    }



    bool sw_em;
    int iter = 0;
    do {
        sw_em = true;
        
        // E step
        double s = 0;
        vector<double> sum;
        for (int i = 0; i < n; ++i) { //Pereschityvaem g[i][j] i sravnivaem s predydushchim shagom. Esli malo otlichayutsya, to algoritm zakanchivaetsya
            s = 0;
            for (int c = 0; c < k; ++c) {
                s += w[c] * N (points[i], mu[c], sigma[c]);
            }
            sum.push_back (s);
        }
        for (int i = 0; i < n; ++i) {
            for (int c = 0; c < k; ++c) {
                if (((g[i][c] - w[c] * N (points[i], mu[c], sigma[c]) / sum[i]) > EPS)
                    || ((g[i][c] - w[c] * N (points[i], mu[c], sigma[c]) / sum[i]) < -EPS)) {
                    sw_em = false;
                }
                g[i][c] = w[c] * N (points[i], mu[c], sigma[c]) / sum[i];
            }
        }
        
        // M step
        double m_c;
        for (int c = 0; c < k; ++c) { //Pereschityvaem w, mu, sigma
            m_c = 0;
            mu[c][0] = 0;
            mu[c][1] = 0;
            for (int i = 0; i < n; ++i) {
                m_c += g[i][c];
            }
            for (int i = 0; i < n; ++i) {
                mu[c][0] += points[i].GetX() * g[i][c] / m_c;
                mu[c][1] += points[i].GetY() * g[i][c] / m_c;
            }
            vector<vector<double>> a (2, vector<double> (2, 0));
            for (int i = 0; i < n; ++i) {
                a[0][0] += g[i][c] * (points[i].GetX() - mu[c][0])
                    * (points[i].GetX() - mu[c][0]);
                a[0][1] += g[i][c] * (points[i].GetX() - mu[c][0])
                    * (points[i].GetY() - mu[c][1]);
                a[1][0] += g[i][c] * (points[i].GetY() - mu[c][1])
                    * (points[i].GetX() - mu[c][0]);
                a[1][1] += g[i][c] * (points[i].GetY() - mu[c][1])
                    * (points[i].GetY() - mu[c][1]);
            }
            sigma[c][0] = a[0][0] / m_c;
            sigma[c][1] = a[0][1] / m_c;
            sigma[c][2] = a[1][0] / m_c;
            sigma[c][3] = a[1][1] / m_c;
            w[c] = m_c / n;
        }
        for (int p = 0; p < n; ++p) {//nahodim v kakoj klaster tochka popadaet s bol'shej veroyatnost'yu
            int ind = 0;
            for (int j = 0; j < k; j++) P[j].assign_point_indicator(p, 0);
            for (int c = 0; c < k; ++c) {
                if (g[p][c] > g[p][ind]) {
                    ind = c;
                }
            }
            P[ind].assign_point_indicator(p, 1);
        }
        print_iter(P, field, iter);
        iter++;
       // em_fprintf (iteration, sigma, mu, g);
    } while (!sw_em);

    //for (int p = 0; p < n; ++p) {//nahodim v kakoj klaster tochka popadaet s bol'shej veroyatnost'yu
    //    int ind = 0;
    //    for (int c = 0; c < k; ++c) {
    //        if (g[p][c] > g[p][ind]) {
    //            ind = c;
    //        }
    //    }
    //    P[ind].assign_point_indicator(p, 1);
    //}
    gif(iter - 1);
    for (int i = 0; i < k; i++)
    {
        fc.add_cluster(P[i]);
    }

    return fc;
}
void EM::print_iter(vector <Cluster> P, Field* field, int iter)
{
    string s_1 = to_string(iter),
        s_2("Algorithms//em//em_");
    s_2.append(s_1);
    s_2.append(".txt");
    ofstream f(s_2);
    for (int i = 0; i < k; i++) P[i].print_cluster(f, field);
    f.close();
}
void EM::gif(int iter)
{
    string Animate = "Algorithms//em//em_Animate.plt";
    ofstream Anim;
    Anim.open(Animate);
    Anim << "set xrange[-15:20] " << endl;
    Anim << "set yrange[-15:20] " << endl;
    Anim << "set term gif animate optimize delay 10 background \"#ffeedf\" font \" Times-Roman,10 \" " << endl;
    Anim << "set output \"em_algoritm.gif\" " << endl;
    Anim << "set size square" << endl;
    Anim << "set palette" << endl;
    Anim << "do for[i=0:" << iter << "]{" << endl;
    Anim << "plot ";
    Anim << "'em_'.i.'.txt' palette notitle" << endl;
    Anim << "}" << endl;
    Anim.close();
}

double EM::N (Point a, vector<double> m, vector<double> Sgm) { //Plotnost' normal'nogo raspredeleniya
    double det = Sgm[0] * Sgm[3] - Sgm[1] * Sgm[2];
    vector<vector<double>> b_s (2, vector<double> (2));
    b_s[0][0] = Sgm[3] / det;
    b_s[0][1] = -Sgm[1] / det;
    b_s[1][0] = -Sgm[2] / det;
    b_s[1][1] = Sgm[0] / det;
    if (det < 0) { det = -det; }
    return exp (-(b_s[0][0] * (a.GetX () - m[0]) * (a.GetX() - m[0])
        + (b_s[1][0] + b_s[0][1]) * (a.GetX() - m[0]) * (a.GetY() - m[1])
        + b_s[1][1] * (a.GetY() - m[1]) * (a.GetY() - m[1])) / 2) / (sqrt (2 * 3.1415 * det));
}

//void em::em_fprintf (vector <Point> points,int iteration,
//                     vector<vector<double> > sgm,
//                     vector<vector<double> > m,
//                     vector<vector<double> > g) const {
//    ofstream out ("gnuplot/em/em" + to_string (iteration) + ".txt");
//    ofstream final ("gnuplot/em/em_fin.txt");
//    for (int p = 0; p < n; ++p) {
//        int ind = 0;
//        for (int c = 0; c < k; ++c) {
//            if (g[p][c] > g[p][ind]) {
//                ind = c;
//            }
//        }
//        out << points[p].GetX() << " " << points[p].GetY() << " " << ind << endl;
//        final << points[p].GetX() << " " << points[p].GetY() << " " << ind << endl;
//    }
//    ofstream ellipsis ("gnuplot/em/ellipse" + to_string (iteration) + ".txt");
//    for (int c = 0; c < k; ++c) {
//        double discr, angle; // discriminant
//        vector<double> lambda;
//        vector<double> v;
//        discr = (sgm[c][0] + sgm[c][3]) * (sgm[c][0] + sgm[c][3]) - 4 * (sgm[c][0] * sgm[c][3] - sgm[c][1] * sgm[c][2]);
//        if (discr >= 0) {
//            lambda.push_back ((sgm[c][0] + sgm[c][3] + sqrt (discr)) / 2);
//            lambda.push_back ((sgm[c][0] + sgm[c][3] - sqrt (discr)) / 2);
//        }
//        v.push_back (sgm[c][0] - lambda[0]);
//        v.push_back (sgm[c][1]);
//        if (v[1] < 0) {
//            v[0] *= -1;
//            v[1] *= -1;
//        }
//        angle = acos (v[0] / sqrt (v[0] * v[0] + v[1] * v[1]));
//        angle *= 180;
//        angle /= M_PI;
//        ellipsis << m[c][0] << " " << m[c][1] << " " << 100 * lambda[0] << " " << 100 * lambda[1] << " " << angle
//                 << endl;
//    }
//}
