#include <target.h>

target::target()
{
    type = -1;
}

target::target(const DoorDetector::Door &xd) {
    coords = Eigen::Vector3f(xd.center_floor.x(), xd.center_floor.y(), 0);
    //update_coords(1.5);
    type = 81;
    is_door = true;
}

target::target(const RoboCompYoloObjects::TBox &dx) {
    coords = Eigen::Vector3f(dx.x, dx.y, 0);
    type = dx.type;
    is_door = false;

}

Eigen::Vector3f target::get_coords() const{
    return coords;
}

void target::update_coords(int factor) {
    if (coords[0] > coords[1])
        coords[0] += factor;
    else
        coords[1] += factor;
}

void target::update_coords(float factor) {
    coords[0] *= factor;
    coords[1] *= factor;
}

int target::get_type() const{
    return type;
}

std::vector<target> target::create_target_list(const std::vector<DoorDetector::Door> &doors) {
    std::vector<target> targets;
    for (auto door : doors) {
        targets.push_back(target(door));
    }
    return targets;
}

std::vector<target> target::create_target_list(const std::vector<RoboCompYoloObjects::TBox> &boxes) {
    std::vector<target> targets;
    for (auto &box : boxes) {
        targets.push_back(target(box));
    }
    return targets;
}
