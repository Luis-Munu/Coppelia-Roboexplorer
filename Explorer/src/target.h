#ifndef TARGET_H
#define TARGET_H

#include <Eigen/Dense>
#include <door_detection.h>
#include <YoloObjects.h>

class target {
    private:
        Eigen::Vector3f coords;
        int type;
        bool is_door;
    public:
        target();
        target(const DoorDetector::Door &xd);
        target(const RoboCompYoloObjects::TBox &dx);
        Eigen::Vector3f get_coords() const;
        void update_coords(float factor);
        void update_coords(int factor);
        int get_type() const;
        std::vector<target> create_target_list(const std::vector<DoorDetector::Door> &doors);
        std::vector<target> create_target_list(const std::vector<RoboCompYoloObjects::TBox> &boxes);
    
};

#endif