#include "Interface.h"

Interface::Interface(bool write_log, Controller *c)
    :write_log(write_log), cc(c) {
    if (write_log) {
        log_file_name = "log.txt";
        logger = ofstream(log_file_name);
        if (!logger.is_open()) {
            cout << "Error while writing logs" << endl;
            this->write_log = false;
        }
    }
}

Interface::~Interface() {
    if (logger.is_open()) {
        logger.close();
    }
}

int Interface::run() {
    // the function asks commands from console until gets the command exit
    cout << "Type HELP to see the list of supported commands." << endl;
    string cur_command; // command with parameters
    getline(cin, cur_command);
    while (manager(cur_command) != -1) {
        getline(cin, cur_command);
    }
    return 0;
}

void Interface::log(const string& s) {
    // writes s to log file
    if (write_log) {
        logger << s << endl;
    }
}

void Interface::show(const string& s) {
    // writes s to console and to log file
    cout << s << endl;
    log("< " + s);
}

int Interface::run(ifstream& inp) {
    /* runs the program from filestream
     * -1 - file not found
     */
    if (!inp.is_open()) {
        show("File not found");
        return -1;
    }
    string cur_command_str;
    getline(inp, cur_command_str);
    while (manager(cur_command_str) != -1 && !inp.eof()) {
        getline(inp, cur_command_str);
    }
    return 0;
}

int Interface::manager(const string& cur_command) {
    /* gets command for controller and sends it;
     * returns -1 for exit;
     */
    log("> " + cur_command);
    cout << cur_command << endl;
    stringstream ss(cur_command);
    string main;
    ss >> main;
    for (auto& c : main) c = toupper(c);
    if (main == "HELP") {
        // output of manual
        string manual;
        cc->get_help(manual);
        log("help.txt");
            ifstream help(manual);
            if (!help.is_open()) {
                show("Manual file not found");
                log("$ " + manual);
            }
            else {
                string cur_line;
                log("* file print:");
                while (getline(help, cur_line)) {
                    printf("%s\n", cur_line.c_str());
                    log("< " + cur_line);
                }
                help.close();
            }
        
    }
    else if (main == "GC") {
        // generation of cloud(s)
            double x, y, disp_x, disp_y;
            int dots = 100;
            ss >> x >> y >> disp_x >> disp_y >> dots;
           // show("gc " + to_string(x) + " " + to_string(y) + " " + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(dots));
            int err = cc->add_cloud_in_field(x, y, disp_x, disp_y, dots);
            if (err == -1)
            {
                show(" Error: field in readonly mode");
            }
            else
            {
                show("Cloud created");
            }
        
    }
    else if (main == "LOG") {
        string b;
        ss >> b;
        if (b == "1") {
            write_log = true;
            show("Writing logs started");
        }
        else if (b == "0") {
            write_log = false;
            show("Writing logs stopped");
        }
        else if (write_log) {
            show("Logs are being written");
        }
        else {
            show("Logs are not being written");
        }
    }
    else if (main == "PRINTF") {
        int err = cc->print_field();
        if (err == -1)
        {
            show("Field doesn't exist.");
        }
        else if (err == -2) {
            show("Field is not in readonly state");
        }
        else
        {
            show("field printed");
        }
    }
    else if (main == "HISTOGRAM") {
        int pieces;
        ss >> pieces;
       int err = cc->creat_field_histogram(pieces);
       if (err == -1)
       {
           show("Field doesn't exist.");
       }
       else if (err == -2) {
           show("Field is not in readonly state");
       }
       else {
           show("Histogram saved to Field histogram for first coordinate.txt and Field histogram for second coordinate.txt");
       }
    }
    else if (main == "EXIT") {
        show("Finishing...");
        return -1;
    }
    else if (main == "BUFFER") {
        string operation;
        ss >> operation;
        for (auto& c : operation) c = toupper(c);
        if (operation == "UNLOAD") {
            int err = cc->upload_all_clouds_from_buffer_in_field();
            if (err == -1) {
                show("Field in readonly mode");
            }
            else {
                show("Buffer unloaded to the field");
            }
        }
        else if (operation == "ADD") {
            int cloud_id;
            ss >> cloud_id;
            int err = cc->add_cloud_from_field_in_buffer(cloud_id);
            if (err == -1)
            {
                show("Field doesn't exist.");
            }
            else if (err == -2) {
                show("Field in readonly mode");
            }
            else {
                show("Cloud ¹" + to_string(cloud_id) + " added to buffer");
            }
        }
        else if (operation == "ROTATE") {
            double angle;
            ss >> angle;
            int err = cc->turn_all_clouds_in_bufffer(angle);
            if (err == -1) {
                show("Field in readonly mode");
            }
            else {
                show("Buffer rotated");
            }
        }
        else if (operation == "ZOOM") {
            double k;
            ss >> k;
            int err = cc->compression_all_clouds_in_buffer(k);
            if (err == -1) {
                show("Field in readonly mode");
            }
            else {
                show("Buffer zoomed");
            }
        }
        else if (operation == "SHIFT") {
            double x, y;
            ss >> x >> y;
            int err = cc->shift_all_clouds_in_buffer(Point(x, y));
            if (err == -1) {
                show("Field in readonly mode");
            }
            else {
                show("Buffer shifted");
            }
        }
        else {
            // something went wrong
            show("Unknown buffer operation. Check your input and try again.");
            show("Type HELP to see the list of supported commands");
        }
    }
    else if (main == "MATRIX") {
        if (cc->matrix() == -1) {
            show("Field not found");
        }
        else {
            show("Field is readonly now. You can analyze it.");
            show("Type HELP to see what you can do");
        }
    }
    else if (main == "BINARY") {
    //creating matrix of incidences
    double delta;
    ss >> delta;
    int err = cc->binary(delta);
    if (err == 0) {
        show("Incidence matrix is created. Now you can run clustering");
    }
    else if (err == -1) {
        show("Field not found");
    }
    else 
    {
        show("Field is not in readonly state.");
    }
    }
    else if (main == "WAVE") {
        double delta;
        ss >> delta;
        int err = cc->wave(delta);
        //show("Clustering...");
        if (err == 0) {
            show("Field is clustered");
        }
        else if (err == -1) {
            show("Field doesn't exist. Type GC to create the clouds");
        }
        else
        {
            show("Field is not in readonly state.");
        }

    }
    else if (main == "DBSCAN") {
        int k;
        double delta;
        ss >> delta >> k;
       // show("Clustering...");
      int err =  cc->dbscan(delta, k);
      if (err == 0) {
          show("Field is clustered");
      }
      else if (err == -1) {
          show("Field doesn't exist. Type GC to create the clouds");
      }
      else
      {
          show("Field is not in readonly state.");
      }
    }
    else if (main == "INFOFINDCLUSTER" || main == "INFOFC") {
    vector<Find_cluster> searches;
    int err = cc->get_find_clusters(searches);
    if (err == -1)
    {
        show("Field is not in readonly state.");
    }
    else {
        int n = searches.size();
        for (int i = 0; i < n; ++i)
        {
            if (searches[i].get_alg() == "Wave" || searches[i].get_alg() == "forel")
            {
                show(to_string(i) + ": alg - " + searches[i].get_alg() + "; d = " + to_string(searches[i].get_delta()) + "; clusters: "
                    + to_string(searches[i].get_number_clusters_in_find_cluster()));
            }
            if (searches[i].get_alg() == "DBSCAN")
            {
                show(to_string(i) + ": alg - " + searches[i].get_alg() + "; d = " + to_string(searches[i].get_delta()) + "; k = " + to_string(searches[i].get_k()) + "; clusters: "
                    + to_string(searches[i].get_number_clusters_in_find_cluster()));
            }
            if (searches[i].get_alg() == "k means" || searches[i].get_alg() == "EM")
            {
                show(to_string(i) + ": alg - " + searches[i].get_alg() + "; k = " + to_string(searches[i].get_k()) + "; clusters: "
                    + to_string(searches[i].get_number_clusters_in_find_cluster()));
            }
            if (searches[i].get_alg() == "kmcores")
            {
                show(to_string(i) + ": alg - " + searches[i].get_alg() + "; k = " + to_string(searches[i].get_k()) + "; p = " + to_string(searches[i].get_p()) + "; clusters: "
                    + to_string(searches[i].get_number_clusters_in_find_cluster()));
            }
            if (searches[i].get_alg() == "ha")
            {
                show(to_string(i) + ": alg - " + searches[i].get_alg() + "; k = " + to_string(searches[i].get_k()) + "; t = " + to_string(searches[i].get_t()) + "; clusters: "
                    + to_string(searches[i].get_number_clusters_in_find_cluster()));
            }
        }
    }
    }

    else if (main == "STREE") {
        // searching minimal spanning tree
      //  show("Clustering...");
      int err = cc->tree();
      if (err == 0) {
          show("Spanning tree completed");
      }
      else if (err == -1) {
          show("Field doesn't exist. Type GC to create the clouds");
      }
      else
      {
          show("Field is not in readonly state.");
      }
    }
    else if (main == "KMEANS") {
        int k;
        ss >> k;
      //  show("Clustering...");
       int err =  cc->k_m(k);
       if (err == 0) {
           show("Field is clustered");
       }
       else if (err == -1) {
           show("Field doesn't exist. Type GC to create the clouds");
       }
       else
       {
           show("Field is not in readonly state.");
       }
    }
    else if (main == "KMCORES") {
        // K-means with cores
        int clusters_number = 0;
        int cores_number = 0;
        ss >> clusters_number >> cores_number;
      //  show("Clustering...");
       int err =  cc->kmcore(clusters_number, cores_number);
       if (err == 0) {
           show("Field is clustered");
       }
       else if (err == -1) {
           show("Field doesn't exist. Type GC to create the clouds");
       }
       else
       {
           show("Field is not in readonly state.");
       }
    }
    else if (main == "EM") {
        int k;
        ss >> k;
     //   show("Clustering...");
       int err =  cc->em_(k);
       if (err == 0) {
           show("Field is clustered");
       }
       else if (err == -1) {
           show("Field doesn't exist. Type GC to create the clouds");
       }
       else
       {
           show("Field is not in readonly state.");
       }

    }
    else if (main == "HA") {
    int k;
    int t;
    ss >> k >> t;
     //   show("Clustering...");
      int err =   cc->hierarchy(k, t);
      if (err == 0) {
          show("Field is clustered");
      }
      else if (err == -1) {
          show("Field doesn't exist. Type GC to create the clouds");
      }
      else
      {
          show("Field is not in readonly state.");
      }
    }
    else if (main == "FOREL") {
    double R;
    ss >> R;
      //  show("Clustering...");
      int err =   cc->forel(R);
      if (err == 0) {
          show("Field is clustered");
      }
      else if (err == -1) {
          show("Field doesn't exist. Type GC to create the clouds");
      }
      else
      {
          show("Field is not in readonly state.");
      }
    }
    else if (main == "DELAUNAY") {
        
   int err =  cc->tri_delaunay();
      if (err == 0) {
       show("Triangulation completed");
      }
      else if (err == -1)
      {
         show("Not enough points");
      }
      else
      {
          show("Field is not in readonly state.");
      }


    }
    else if (main == "INTER") {
    double x, y, f = 0;
    ss >> x >> y;
    Point p(x, y);
   // show("Interpolation...");
   int err =  cc->function_interpolation(p, f);
   if(err == 0)
   {
       show("Interpolation completed " + to_string(f));
   }

   else if(err == -2)
   {
       show("Error. Points is too far away from the clusters.");
   }
   else if(err == -3)
   {
       show("big err");
   }
   else
   {
       show("Field is not in readonly state.");
   }

    }
    else if (main == "#") {
        // doing nothing, that's comment
    }
    else {
        // something went wrong
        show("Unknown command. Check your input and try again.");
        show("Type HELP to see the list of supported commands");
    }
    return 0;
}
