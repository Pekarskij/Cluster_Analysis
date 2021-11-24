#include "Controller.h"

Controller::Controller() { analysis_state = false; number_find_clusters_in_field = 0;
}

Controller::Controller(const Controller& c) {
    analysis_state = c.analysis_state; field = c.field; number_find_clusters_in_field = c.number_find_clusters_in_field;
}

const Controller& Controller::operator=(const Controller& c)
{
    analysis_state = c.analysis_state;
    number_find_clusters_in_field = c.number_find_clusters_in_field;
    field = c.field;
    find_clusters = c.find_clusters;
    return *this;
}

void Controller::add_cloud_in_field(Cloud c) { field.add_cloud_in_field(c); }

void Controller::delete_cloud_from_field(int id) { field.delete_cloud_from_field(id); }

void Controller::print_field() { field.print_field(); }

bool Controller::get_analysis_state() { return analysis_state; }
vector <Find_cluster> Controller::get_find_clusters() { return find_clusters; }

void Controller::add_cloud_from_field_in_buffer(int id) { field.add_cloud_from_field_in_buffer(id); }

void Controller::add_all_clouds_from_field_in_buffer() { field.add_all_clouds_from_field_in_buffer(); }

void Controller::upload_cloud_from_buffer_in_field(int n) { field.upload_cloud_from_buffer_in_field(n); }

void Controller::upload_all_clouds_from_buffer_in_field() { field.upload_all_clouds_from_buffer_in_field(); }

void Controller::shift_cloud_in_buffer(int n, Point v) { field.shift_cloud_in_buffer(n, v); }

void Controller::shift_all_clouds_in_buffer(Point v) { field.shift_all_clouds_in_buffer(v); }

void Controller::turn_cloud_in_bufffer(int n, double phi) { field.turn_cloud_in_bufffer(n, phi); }

void Controller::turn_all_clouds_in_bufffer(double phi) { field.turn_all_clouds_in_bufffer(phi); }

void Controller::compression_cloud_in_buffer(int n, double lambda) { field.compression_cloud_in_buffer(n, lambda); }

void Controller::compression_all_clouds_in_buffer(double lambda) { field.compression_all_clouds_in_buffer(lambda); }

void Controller::creat_field_histogram(int number_of_columns) { field.creat_field_histogram(number_of_columns); }

void Controller::creat_cloud_histogram(int id, int number_of_colunms) { field.creat_cloud_histogram(id, number_of_colunms); }

void Controller::wave(double delta) {
analysis_state = true;
int c = 0;
Wave_algorithm w;
vector <Point> points = field.get_points_from_field();
number_find_clusters_in_field++;
w.assign_delta(delta);
find_clusters.push_back(w.find_clusters(points));
find_clusters[number_find_clusters_in_field - 1].assign_alg("Wave");
for (int i = 0; i < number_find_clusters_in_field - 1; i++)
{
    if (find_clusters[i].get_alg() == "Wave") c++;
}
find_clusters[number_find_clusters_in_field - 1].assign_delta(delta);
find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
find_clusters[number_find_clusters_in_field - 1].print_wave_algorithm(points);
}

void Controller::forel(double R) {
analysis_state = true;
int c = 0;
FOREL f;
vector <Point> points = field.get_points_from_field();
number_find_clusters_in_field++;
f.assign_R(R);
find_clusters.push_back(f.find_clusters(points));
find_clusters[number_find_clusters_in_field - 1].assign_alg("forel");
for (int i = 0; i < number_find_clusters_in_field - 1; i++)
{
    if (find_clusters[i].get_alg() == "forel") c++;
}
find_clusters[number_find_clusters_in_field - 1].assign_delta(R);
find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
find_clusters[number_find_clusters_in_field - 1].print_forel_algorithm(points);
}
void Controller::dbscan(double delta, int k) {
 analysis_state = true;
 int c = 0;
 DBSCAN db;
 vector <Point> points = field.get_points_from_field();
 vector <int> cpd;
 number_find_clusters_in_field++;
 db.assign_delta(delta);
 db.assign_k(k);
 find_clusters.push_back(db.find_clusters(points, cpd));
 find_clusters[number_find_clusters_in_field - 1].assign_alg("DBSCAN");
 for (int i = 0; i < number_find_clusters_in_field - 1; i++)
 {
     if (find_clusters[i].get_alg() == "DBSCAN") c++;
 }
 find_clusters[number_find_clusters_in_field - 1].assign_delta(delta);
 find_clusters[number_find_clusters_in_field - 1].assign_k(k);
 find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
 find_clusters[number_find_clusters_in_field - 1].print_dbscan_algorithm(points, cpd);
}

void Controller::k_m(int k) {
 analysis_state = true;
 int c = 0;
 k_means km;
 vector <Point> points = field.get_points_from_field();
 number_find_clusters_in_field++;
 km.assing_k(k);
 find_clusters.push_back(km.find_clusters(points));
 find_clusters[number_find_clusters_in_field - 1].assign_alg("k means");
 for (int i = 0; i < number_find_clusters_in_field - 1; i++)
 {
     if (find_clusters[i].get_alg() == "k means") c++;
 }
 find_clusters[number_find_clusters_in_field - 1].assign_k(k);
 find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
 find_clusters[number_find_clusters_in_field - 1].print_k_means_algorithm(points);
}

void Controller::kmcore(int k, int p) {
    analysis_state = true;
    int c = 0;
    kmcores km;
    vector <Point> points = field.get_points_from_field();
    number_find_clusters_in_field++;
    km.assing_k(k);
    km.assing_p(p);
    find_clusters.push_back(km.find_clusters(points));
    find_clusters[number_find_clusters_in_field - 1].assign_alg("kmcores");
    for (int i = 0; i < number_find_clusters_in_field - 1; i++)
    {
        if (find_clusters[i].get_alg() == "kmcores") c++;
    }
    find_clusters[number_find_clusters_in_field - 1].assign_k(k);
    find_clusters[number_find_clusters_in_field - 1].assign_p(p);
    find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
    find_clusters[number_find_clusters_in_field - 1].print_kmcores_algorithm(points);
}
void Controller::em(int k) {
    analysis_state = true;
    int c = 0;
    EM em;
    vector <Point> points = field.get_points_from_field();
   
    number_find_clusters_in_field++;
  
    em.assign_k(k);
    
    find_clusters.push_back(em.find_clusters(points));
    
    find_clusters[number_find_clusters_in_field - 1].assign_alg("EM");
   
    for (int i = 0; i < number_find_clusters_in_field - 1; i++)
    {
        if (find_clusters[i].get_alg() == "EM") c++;
    }
    find_clusters[number_find_clusters_in_field - 1].assign_k(k);
    
    find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
    
    find_clusters[number_find_clusters_in_field - 1].print_em_algorithm(points);
   
}

void Controller::tree() {
    analysis_state = true;
    Spanning_tree T;
    vector <Point> points = field.get_points_from_field();
    vector <pair <double, pair <int, int>>> edges;

    edges = T.creat_spanning_tree(points);
    T.print_spanning_tree(points, edges);
}

void Controller::hierarchy(int k, int t) {
    analysis_state = true;
    int c = 0;
    ha h;
    vector <Point> points = field.get_points_from_field();
    number_find_clusters_in_field++;
    h.assing_k(k);
    h.assing_t(t);
    find_clusters.push_back(h.find_clusters(points));
    find_clusters[number_find_clusters_in_field - 1].assign_alg("ha");
    for (int i = 0; i < number_find_clusters_in_field - 1; i++)
    {
        if (find_clusters[i].get_alg() == "ha") c++;
    }
    find_clusters[number_find_clusters_in_field - 1].assign_k(k);
    find_clusters[number_find_clusters_in_field - 1].assign_t(t);
    find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
    find_clusters[number_find_clusters_in_field - 1].print_ha_algorithm(points);
}

void Controller::tri_delaunay()
{
    analysis_state = true;
    vector <Point> points = field.get_points_from_field();
    delaunay tri;
    tri.delaunay_(points);
}
void Controller::get_help(string& file_name) {
    file_name = "help.txt";
}