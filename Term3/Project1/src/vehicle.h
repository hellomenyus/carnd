#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <chrono>

using namespace std;

class TrajectoryData {
public:
    int proposed_lane;
    double avg_speed;
    int max_accel;
    double rms_acceleration;
    double closest_approach;
    int end_distance_to_goal;
    int end_lanes_from_goal;
    int collides;
};

class Vehicle {
public:
    int lane;

    double s;

    double v;

    double a;

    chrono::time_point<chrono::system_clock> last_update;

    double target_speed;

    int lanes_available;

    double max_acceleration;

    int goal_lane;

    int goal_s;

    string state;

    /**
    * Constructor
    */
    Vehicle(int lane, double s, double v, double a);

    /**
    * Destructor
    */
    virtual ~Vehicle();

    void update_state(map<int, vector <vector<double> > > predictions, int horizon = PLANNING_HORIZON);

    void configure(double target_speed, int lanes_available, double max_acceleration, int goal_lane, int goal_s);

    string display();

    void increment(double dt);

    vector<double> state_at(int t);

    void realize_state(map<int, vector < vector<double> > > predictions);

    void realize_constant_speed();

    double _max_accel_for_lane(map<int,vector<vector<double> > > predictions, int lane, double s);

    void realize_keep_lane(map<int, vector< vector<double> > > predictions);

    void realize_lane_change(map<int,vector< vector<double> > > predictions, string direction);

    void realize_prep_lane_change(map<int,vector< vector<double> > > predictions, string direction);

    vector<vector<double> > generate_predictions(int horizon = PLANNING_HORIZON);

    // new functions:

    Vehicle clone()
    {
        Vehicle c(this->lane, this->s, this->v, this->a);
        c.target_speed = this->target_speed;
        c.lanes_available = this->lanes_available;
        c.max_acceleration = this->max_acceleration;
        c.goal_lane = this->goal_lane;
        c.goal_s = this->goal_s;
        c.state = this->state;
        return c;
    };

    double calculate_cost(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions, string state);
    TrajectoryData get_helper_data(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions);
    bool check_collision(Vehicle snapshot, double s_previous, double s_now);
    double distance_from_goal_lane(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions, TrajectoryData data);
    double inefficiency_cost(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions, TrajectoryData data);
    double collision_cost(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions, TrajectoryData data);
    double change_lane_cost(vector<Vehicle> trajectory, map<int,vector<vector<double>>> predictions, TrajectoryData data);
    double state_cost(string state);

private:
    static const int PLANNING_HORIZON = 6;
    static const int KEEP_LANE_PREFERRED_BUFFER = 20;
    static const int CHECK_COLLISION_PREFERRED_BUFFER_FRONT = 40;
    static const int CHECK_COLLISION_PREFERRED_BUFFER_BACK = 4;

    static const long long REACH_GOAL = pow(10, 1);
    static const long long EFFICIENCY = pow(10, 2);
    static const long long COMFORT    = pow(10, 4);
    static const long long DANGER     = pow(10, 6);
    static const long long COLLISION  = pow(10, 8);
    static const long long INFINITY_COST  = pow(10, 12);

};

#endif
