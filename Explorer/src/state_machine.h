#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Eigen/Dense>
#include <robot.h>
#include <target.h>
#include <timer/timer.h>
#include <graph.h>

class state_machine
{
private:
    enum class State {SEARCHING, APPROACHING, CROSSING, SCANNING, UNSTUCK};
    State state = State::SCANNING;
    Graph grafo = Graph();
    int last_room_id = 0;
    Graph::Room room;
    bool first_time = true;
    float old_dist = 0.0f;


public:
    void update(const std::vector<target> &objects, const std::vector<Eigen::Vector2f> &line, rc::Robot &robot, AbstractGraphicViewer *viewer);
    bool time_check(int time_interval);
    void search_state(const std::vector<target> &objects,rc::Robot &robot, AbstractGraphicViewer *viewer);
    void approach_state(const std::vector<target> &objects, const std::vector<Eigen::Vector2f> &line, rc::Robot &robot, AbstractGraphicViewer *viewer);
    void cross(rc::Robot &robot);
    void scan_room(const std::vector<target> &objects,rc::Robot &robot);
    void unstuck(rc::Robot &robot);
    
};




#endif