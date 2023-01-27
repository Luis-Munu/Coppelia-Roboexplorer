#include "state_machine.h"

static rc::Timer click;

bool state_machine::time_check(int time_interval){
    if (first_time)
    {
      click.tick();        
      first_time = false;
    }
    else{
        click.tock();     
        if(click.duration() >= time_interval)
        {
            first_time = true; 
            return false; 
        }
    }
    return true;
}


void state_machine::search_state(const std::vector<target> &objects, rc::Robot &robot, AbstractGraphicViewer *viewer){
    std::cout << "searching state" << std::endl;
    robot.set_rot_tychon(-1.5f);
    for (const auto &obj : objects)
    {
        if (obj.get_type() == 81){
            robot.set_current_targeto(obj);
            robot.set_has_target(true);
            robot.set_rot_tychon(0.0f);
            first_time = true;
            old_dist = robot.get_distance_to_target();
            state = State::APPROACHING;

            return;
        }
    }     

    if (!time_check(4000)){
        state = State::UNSTUCK;
        robot.set_rot_tychon(0.0f);
        return;
    }
}


void state_machine::approach_state(const std::vector<target> &objects, const std::vector<Eigen::Vector2f> &line, rc::Robot &robot, AbstractGraphicViewer *viewer)
{
    /* Method used to approach the door by updating its coordinates */ 
    std::cout << "Approaching item with ID: " << robot.get_current_target().get_type() << " and distance: " << robot.get_distance_to_target() << std::endl;

    bool cross_flag = false;

    if (!time_check(12000)){
        state = State::UNSTUCK;
        return;
    }
    // delete all objects which distance is bigger than 1000 of the current target
    std::vector<target> filtered_objects = objects;

    filtered_objects.erase(std::remove_if(filtered_objects.begin(), filtered_objects.end(), [&robot](const target &obj) {
        return (obj.get_coords() - robot.get_current_target().get_coords()).norm() > 1000;
    }), filtered_objects.end());
    
    // get the object closest to the last target
    auto closest = std::min_element(filtered_objects.begin(), filtered_objects.end(), [&robot](const target &obj1, const target &obj2) {
        return (obj1.get_coords() - robot.get_current_target().get_coords()).norm() < (obj2.get_coords() - robot.get_current_target().get_coords()).norm();
    });

    old_dist = robot.get_distance_to_target();

    if (closest != filtered_objects.end() and (closest->get_coords() - robot.get_current_target().get_coords()).norm() < 1000)
    {
        robot.set_current_targeto(*closest);
        robot.set_has_target(true);
    }
    else if (robot.get_distance_to_target() < 750)
        cross_flag = true;
    if (robot.get_distance_to_target() < 400)
        cross_flag = true;

    if (cross_flag)
    {
        state = State::CROSSING;
        robot.set_has_target(false);
        robot.set_rot_tychon(0.0f);
        first_time = true;
        cross_flag = false;
        return;
    }
}

void state_machine::cross(rc::Robot &robot)
{
    /* Method used to cross the door by moving straight for 3 seconds */

    //if (first_time)
        //robot.get_current_target().update_coords(1.25);
    robot.set_adv_tychon(700.0f);
    if (!time_check(2000)){
        state = State::SCANNING;
        robot.set_adv_tychon(0.0f);
    }

    std::cout << "CROSSING" << std::endl;

}

void state_machine::scan_room(const std::vector<target> &objects, rc::Robot &robot){
    std::cout << "Scanning" << std::endl;

    robot.set_rot_tychon(-1.5f);
    // create a virtual Room

    //lambda to check if an object exists in the room by type, if it does, it compares the coordinates and if the distance is greater than 750, it adds it to the room
    // if the object id isn't in the room, it returns true
    auto check = [this](const target &obj) {
        std::pair<int, Eigen::Vector2f> item = std::make_pair(obj.get_type(), Eigen::Vector2f(obj.get_coords().x(), obj.get_coords().y()));

        //check if there is any pair in the room with the same id, return true if there isn't, otherwise check the distance
        if (std::find_if(room.items.begin(), room.items.end(), [&item](const std::pair<int, Eigen::Vector2f> &p) { return p.first == item.first; }) == room.items.end())
            return true;
        else
            return (item.second - std::find_if(room.items.begin(), room.items.end(), [&item](const std::pair<int, Eigen::Vector2f> &p) { return p.first == item.first; })->second).norm() > 750.0f;

    };

    // add items to the room
    for (auto &obj : objects)
        if (robot.get_robot_target_coordinates(obj).norm() < 2000.0f and obj.get_type() != 81 and check(obj)){
            grafo.add_item_to_room(room, obj.get_type(), obj.get_coords());
            printf("Added item with ID %d to the room with ID %d", obj.get_type(), room.id);
        }

    if (!time_check(3900)){
        // Returns the ID of the room, if it doesn't exist it gets inserted on the graph
        int id = grafo.find_room(room);

        // update the links between the rooms
        if(last_room_id != id) {grafo.add_link(std::make_pair(last_room_id, id));}
        last_room_id = id;
        state = State::SEARCHING;           
        robot.set_rot_tychon(0.0f);
        room.items.clear();
        room.id = -1;
    }
}

void state_machine::unstuck(rc::Robot &robot){
    std::cout << "Unstuck" << std::endl;
    // half the time, the robot advances forward, the other half backwards
    if (rand() % 3 != 0)
        robot.set_adv_tychon(-1000.0f);
    else
        robot.set_adv_tychon(1000.0f);
    if (!time_check(3000)){
        state = State::SEARCHING;
        robot.set_adv_tychon(0.0f);
    }
}


void state_machine::update( const std::vector<target> &objects, 
                            const std::vector<Eigen::Vector2f> &line,
                            rc::Robot &robot, AbstractGraphicViewer *viewer)
{
    switch (state)
    {
    case State::SEARCHING:
        search_state(objects,robot, viewer);
        break;
    case State::APPROACHING:
        approach_state(objects,line,robot, viewer);
        break;
    case State::SCANNING:
        scan_room(objects,robot);
        break;
    case State::CROSSING:
        cross(robot);
        break;
    case State::UNSTUCK:
        unstuck(robot);
        break;
    default:
        unstuck(robot);
        break;
    }
}