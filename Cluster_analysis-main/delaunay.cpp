#include "delaunay.h"

void delaunay::delaunay_(vector <Point> points) {
    // Sorting all points by x coordinate
    int n = points.size();
    vector<int> points_sorted;
    for (int i = 0; i < n; ++i) {
        points_sorted.push_back(i);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (points[points_sorted[j]].GetX() > points[points_sorted[i]].GetX()) {
                int temp = points_sorted[i];
                points_sorted[i] = points_sorted[j];
                points_sorted[j] = temp;
            }
        }
    }
    vector<vector<bool>> edge(n, vector<bool>(n, false));
    // creating the first edge
    // getting the first edge is to find the line
    // which satisfies condition: all the field points are on one side
    // we already have sorted points, so it's enough to check the most left and find pair for it
    // it means, we search the most vertical vector here

    int min_i = 1;
    double min_angle = 90;
    for (int i = 1; i < n; ++i) {
        // it's good idea to draw that points to understand what does temp here
        Point temp = Point(points[points_sorted[0]].GetX(),
            points[points_sorted[i]].GetY());
        vector <Point> temp_edge = { temp, points[points_sorted[i]] };
        double curr_angle = abs(delaunay_angle_to_edge(temp_edge, points[points_sorted[0]]));
        if (min_angle > curr_angle) {
            min_angle = curr_angle;
            min_i = i;
        }
    }

    vector<vector <Point>> baselines;
    if (points[points_sorted[0]].GetY() > points[points_sorted[min_i]].GetY()) {
        vector <Point> edge_ = { points[points_sorted[min_i]], points[points_sorted[0]] };
        baselines.push_back(edge_);
    }
    else {
        vector <Point> edge_ = { points[points_sorted[0]], points[points_sorted[min_i]] };
        baselines.push_back(edge_);
    }
    edge[points_sorted[0]][points_sorted[min_i]] = edge[points_sorted[min_i]][points_sorted[0]] = true;
    delaunay_base_run(baselines, edge, points);
    delaunay_fprintf(edge, points);
}

void delaunay::delaunay_base_run(vector<vector <Point>>& baselines,
    vector<vector<bool>>& edge, vector <Point> points) {
    // The function has no mechanics for cases when there is no satisfying points

    int max_i;
    double max_angle;
    bool started = false;
    max_angle = 0;
    for (int i = 0; i < points.size(); ++i) {
        //checking if the point is on the right side from edge
        Point vec_1(baselines[0][1].GetX() - points[i].GetX(), baselines[0][1].GetY() - points[i].GetY());
        Point vec_2(baselines[0][0].GetX() - points[i].GetX(), baselines[0][0].GetY() - points[i].GetY());
        if (!((vec_1.GetX() == 0 && vec_1.GetY() == 0) || (vec_2.GetX() == 0 && vec_2.GetY() == 0))) {
            if (vec_1.GetX() * vec_2.GetY() > vec_2.GetX() * vec_1.GetY()) {
                double curr_angle = delaunay_angle_to_edge(baselines[0], points[i]);
                if (max_angle < curr_angle) {
                    started = true;
                    max_angle = curr_angle;
                    max_i = i;
                }
            }
        }
    }
    if (started) {
        if (!edge[baselines[0][0].get_Point_id()][max_i]) {
            edge[baselines[0][0].get_Point_id()][max_i] = edge[max_i][baselines[0][0].get_Point_id()] = true;
            vector <Point> edge_ = { baselines[0][0], points[max_i] };
            baselines.push_back(edge_);
        }
        if (!edge[baselines[0][1].get_Point_id()][max_i]) {
            edge[baselines[0][1].get_Point_id()][max_i] = edge[max_i][baselines[0][1].get_Point_id()] = true;
            vector <Point> edge_ = { points[max_i], baselines[0][1] };
            baselines.push_back(edge_);
        }

    }
    baselines.erase(baselines.cbegin());
    if (!baselines.empty()) {
        delaunay_base_run(baselines, edge, points);
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

void delaunay::delaunay_fprintf(vector<vector<bool>>& edge, vector <Point> points) {
    // prints algorithm's state
    ofstream edges("Algorithms//delaunay//dl.txt"), g("Algorithms//delaunay//dl.plt");
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            if (edge[i][j]) {
                edges <<points[i].GetX() << " " <<points[i].GetY() << endl;
                edges << points[j].GetX() << " " << points[j].GetY() << endl << endl;
            }
        }
    }
    g << "set size square\n" << "set palette\n" << "plot 'dl.txt' with lines\n";
}

