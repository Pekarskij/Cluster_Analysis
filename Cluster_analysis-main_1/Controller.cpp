#include "Controller.h"

//Controller::Controller() { analysis_state = false; number_find_clusters_in_field = 0;
//}

//Controller::Controller(const Controller& c) {
//    analysis_state = c.analysis_state; field = c.field; number_find_clusters_in_field = c.number_find_clusters_in_field; 
//}

//const Controller& Controller::operator=(const Controller& c)
//{
//    analysis_state = c.analysis_state;
//    number_find_clusters_in_field = c.number_find_clusters_in_field;
//    field = c.field;
//    find_clusters = c.find_clusters;
//    return *this;
//}

Controller::Controller(bool write_log) : write_log(write_log) {
    if (write_log) {
        log_file_name = "log_ctrl.txt";
        logger = ofstream(log_file_name);
        if (!logger.is_open()) {
            cout << "Error while writing logs" << endl;
            this->write_log = false;
        }
    }
    field = nullptr;
    analysis_state = false;
    number_find_clusters_in_field = 0;
}
Controller::~Controller() {
    if (logger.is_open()) {
        logger.close();
    }
}

int Controller::get_help(string& file_name) {
    log("> help");
    file_name = "help.txt";
    log("< help-fajl otpravlen");
    return 0;
}
void Controller::log(const string& s) {
    if (write_log) {
        logger << s << endl;
    }
}
int Controller::add_cloud_in_field(double x, double y, double disp_x, double disp_y, int dots) {
    log("> gc " + to_string(x) + " " + to_string(y) + " " + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(dots));
    if (field == nullptr) {
        field = new Field();
        log("< Field initialized");
    }
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -1;
    }
    else {

        field->add_cloud_in_field(Cloud(x, y, disp_x, disp_y, dots));
        log("< Cloud created");
        return 0;
    }
}

//void Controller::delete_cloud_from_field(int id) { field->delete_cloud_from_field(id); }

int Controller::print_field() {
    log("> print_field");
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else
    {
        field->print_field();
        log("< complete");
        return 0;
    }
}

bool Controller::get_analysis_state() { return analysis_state; }
int Controller::get_find_clusters(vector <Find_cluster> &fc)
{
    if (!(analysis_state))
    {
    log("< Field is not in readonly state.");
    return -1;
    }
    else {
        fc = find_clusters;
        log("< complete");
        return 0;
    }
}

int Controller::add_cloud_from_field_in_buffer(int id)
{
    log("> add_cloud_from_field_in_buffer");
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -2;
    }
    else
    {
        field->add_cloud_from_field_in_buffer(id);
        log("< complete");
        return 0;
    }
}

int Controller::add_all_clouds_from_field_in_buffer()
{
    log("> add_all_clouds_from_field_in_buffer");
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -2;
    }
    else
    {
        field->add_all_clouds_from_field_in_buffer();
        log("< complete");
        return 0;
    }
}

//void Controller::upload_cloud_from_buffer_in_field(int n) { field->upload_cloud_from_buffer_in_field(n); }

int Controller::upload_all_clouds_from_buffer_in_field() 
{ 
    log("> upload_all_clouds_from_buffer_in_field");
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -1;
    }
    else
    {
        field->upload_all_clouds_from_buffer_in_field();
        log("< complete");
        return 0;
    }
}

//void Controller::shift_cloud_in_buffer(int n, Point v) { field->shift_cloud_in_buffer(n, v); }

int Controller::shift_all_clouds_in_buffer(Point v) 
{ 
    log("> shift_all_clouds_in_buffer");
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -1;
    }
    else
    {
        field->shift_all_clouds_in_buffer(v);
        log("< complete");
        return 0;
    }
}

//void Controller::turn_cloud_in_bufffer(int n, double phi) { field->turn_cloud_in_bufffer(n, phi); }

int Controller::turn_all_clouds_in_bufffer(double phi)
{
    log("> turn_all_clouds_in_bufffer");
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -1;
    }
    else
    {
        field->turn_all_clouds_in_bufffer(phi);
        log("< complete");
        return 0;
    }
}

//void Controller::compression_cloud_in_buffer(int n, double lambda) { field->compression_cloud_in_buffer(n, lambda); }

int Controller::compression_all_clouds_in_buffer(double lambda)
{ 
    log("> compression_all_clouds_in_buffer");
    if (analysis_state)
    {
        log("< Error: field in readonly mode");
        return -1;
    }
    else
    {
        field->compression_all_clouds_in_buffer(lambda);
        log("< complete");
        return 0;
    }
}

int Controller::creat_field_histogram(int number_of_columns) 
{
    log("> histogram");
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else
    {
        field->creat_field_histogram(number_of_columns);
        log("< complete");
        return 0;
    }

}

//void Controller::creat_cloud_histogram(int id, int number_of_colunms) { field->creat_cloud_histogram(id, number_of_colunms); }

int Controller::matrix()
{
    log("> Matrix");
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else
    {
        field->create_dist_mat();
        analysis_state = true;
        log("< Field is readonly now.");
        return 0;
    }
}
int Controller::binary(double delta)
{
    log("> Binary d = " + to_string(delta));
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else
    {
        int c = 0;
        Find_cluster fc;
        fc.create_binary(this->field, delta);
        find_clusters.push_back(fc);
        number_find_clusters_in_field++;
        find_clusters[number_find_clusters_in_field - 1].assign_alg("Binary");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "Binary") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_delta(delta);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_binary_algorithm(this->field);
        log("< complete ");
        return 0;
    }
}

int Controller::wave(double delta) {

 log("> Wave d = " + to_string(delta));

if (field == nullptr)
{
    log("< Field doesn't exist.");
    return -1;
}
else if (!(analysis_state))
{
    log("< Field is not in readonly state.");
    return -2;
}
else
{
    int c = 0;
    number_find_clusters_in_field++;
    w.assign_delta(delta);
    find_clusters.push_back(w.find_clusters(this->field));
    find_clusters[number_find_clusters_in_field - 1].assign_alg("Wave");
    for (int i = 0; i < number_find_clusters_in_field - 1; i++)
    {
        if (find_clusters[i].get_alg() == "Wave") c++;
    }
    find_clusters[number_find_clusters_in_field - 1].assign_delta(delta);
    find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
    find_clusters[number_find_clusters_in_field - 1].print_wave_algorithm(this->field);
    
    log("< Field is clustered");
    return 0;
}
}

int Controller::forel(double R) {

    log("> Forel R = " + to_string(R));
   
    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        int c = 0;
        number_find_clusters_in_field++;
        f.assign_R(R);
        find_clusters.push_back(f.find_clusters(this->field));
        find_clusters[number_find_clusters_in_field - 1].assign_alg("forel");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "forel") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_delta(R);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_forel_algorithm(this->field);
        
        log("< Field is clustered");
        return 0;
    }
}
int Controller::dbscan(double delta, int k) {
    log("> dbscan d = " + to_string(delta) + " k = " + to_string(k));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
     else if(!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else
    {
        int c = 0;
        vector <int> cpd;
        number_find_clusters_in_field++;
        db.assign_delta(delta);
        db.assign_k(k);
        find_clusters.push_back(db.find_clusters(this->field, cpd));
        find_clusters[number_find_clusters_in_field - 1].assign_alg("DBSCAN");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "DBSCAN") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_delta(delta);
        find_clusters[number_find_clusters_in_field - 1].assign_k(k);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_dbscan_algorithm(this->field, cpd);
        
        cpd.clear();
        log("< Field is clustered");
        return 0;
    }
}

int Controller::k_m(int k) {
    log("> kmeans k = " + to_string(k));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        int c = 0;
        number_find_clusters_in_field++;
        km.assing_k(k);
        find_clusters.push_back(km.find_clusters(this->field));
        find_clusters[number_find_clusters_in_field - 1].assign_alg("k means");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "k means") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_k(k);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_k_means_algorithm(this->field);
        
        log("< Field is clustered");
        return 0;
    }
}

int Controller::kmcore(int k, int p) {
    log("> kmcores k = " + to_string(k) + " p = " + to_string(p));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        int c = 0;
        number_find_clusters_in_field++;
        kmc.assing_k(k);
        kmc.assing_p(p);
        vector<vector<Point>> cores;
        find_clusters.push_back(kmc.find_clusters(this->field, cores));
        find_clusters[number_find_clusters_in_field - 1].assign_alg("kmcores");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "kmcores") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_k(k);
        find_clusters[number_find_clusters_in_field - 1].assign_p(p);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_kmcores_algorithm(this->field, cores);
        
        cores.clear();
        log("< Field is clustered");
        return 0;
    }
}
int Controller::em_(int k) {
    log("> em k = " + to_string(k));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        int c = 0;

        number_find_clusters_in_field++;

        em.assign_k(k);

        find_clusters.push_back(em.find_clusters(this->field));

        find_clusters[number_find_clusters_in_field - 1].assign_alg("EM");

        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "EM") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_k(k);

        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);

        find_clusters[number_find_clusters_in_field - 1].print_em_algorithm(this->field);
        
        log("< Field is clustered");
        return 0;
    }
}

int Controller::tree() {
    log("> spanning tree");
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        vector <pair <double, pair <int, int>>> edges;

        edges = T.creat_spanning_tree(this->field);
        T.print_spanning_tree(this->field, edges);
        double d = T.get_delta();
        wave(d);
        
        edges.clear();
        log("< Spanning tree completed");
        return 0;
    }
}

int Controller::hierarchy(int k, int t) {
    log("> ha k = " + to_string(k) + " t = " + to_string(t));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        int c = 0;
        number_find_clusters_in_field++;
        h.assing_k(k);
        h.assing_t(t);
        find_clusters.push_back(h.hierarchy(this->field));
        find_clusters[number_find_clusters_in_field - 1].assign_alg("ha");
        for (int i = 0; i < number_find_clusters_in_field - 1; i++)
        {
            if (find_clusters[i].get_alg() == "ha") c++;
        }
        find_clusters[number_find_clusters_in_field - 1].assign_k(k);
        find_clusters[number_find_clusters_in_field - 1].assign_t(t);
        find_clusters[number_find_clusters_in_field - 1].assign_alg_number(c);
        find_clusters[number_find_clusters_in_field - 1].print_ha_algorithm(this->field);
        
        log("< Field is clustered");
        return 0;
    }
}

int Controller::tri_delaunay()
{
    log("> Delaunay");
    
    if (field->get_number_points_in_field() < 3)
    {
        log("< Not enough points");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -2;
    }
    else {
        vector<Point> points = field->get_points_from_field();
        analysis_state = true;
        tri.delaunay_(points);
        tri.delaunay_fprintf(points);
        
        log("< Field is triangulated");
        return 0;
    }
}

int Controller::function_interpolation(Point p, double &f)
{
    log("> Interpolation " + to_string(p.GetX()) + " " + to_string(p.GetY()));
    

    if (field == nullptr)
    {
        log("< Field doesn't exist.");
        return -1;
    }
    else if (!(analysis_state))
    {
        log("< Field is not in readonly state.");
        return -4;
    }
    else {
        int err = Prediction.interpolation(p, this->field);
        if (err == -2)
        {
            log("< this->field is too far away from the clusters.");
            
            return -2;
        }
        else
        {
            Prediction.find_R(this->field);
            
            double R = Prediction.get_R();
            if (R >= 0.9 && R <= 1)
            {
                f = Prediction.get_f();
                log("< Interpolation completed");
                log("< znachenie funkcii f = " + to_string(f) + " koefficient oshibki R = " + to_string(R));
                return 0;
            }
            else
            {
                log("< slishkom bol'shaya pogreshnost' R = " + to_string(R));
                return -3;
            }
        }
    }
}
//void Controller::get_help(string& file_name) {
//    file_name = "help.txt";
//}
