//
// Created by pbustos on 11/11/22.
//

#ifndef FORCEFIELD_ROBOT_H
#define FORCEFIELD_ROBOT_H

#include <Eigen/Dense>
#include <QtCore>
#include <QColor>
#include <abstract_graphic_viewer/abstract_graphic_viewer.h>
#include "camera.h"
#include <JointMotorSimple.h>
#include <dynamic_window.h>
#include <OmniRobot.h>
#include <target.h>

namespace rc
{
    class Robot
    {
        public:
                Robot() = default;
                void update_speed();
                void update_joints();

                // configuration
                void initialize(RoboCompOmniRobot::OmniRobotPrxPtr omnirobot_proxy_);

                // control
                void set_current_targeto(const target &targeto);
                void goto_target(std::vector<Eigen::Vector2f> current_line, AbstractGraphicViewer *viewer);

                void add_camera(const Eigen::Transform<float, 3, Eigen::Affine> &tf,
                                const std::vector<std::string> &axes_,
                                RoboCompJointMotorSimple::JointMotorSimplePrxPtr joint_proxy);

                Eigen::Vector3f get_robot_target_coordinates();
                Eigen::Vector3f get_robot_target_coordinates(target objetivo);
                Eigen::Vector3f get_camera_target_coordinates() const;
                float get_current_advance_speed() const;
                float get_current_rot_speed() const;
                float get_target_angle_in_frame() const;
                float get_current_pan_angle() const;
                target get_current_target() const;
                float get_distance_to_target();
                Eigen::Transform<float, 3, Eigen::Affine> get_tf_cam_to_base();
                void print();
                void create_bumper(float offset =  150, AbstractGraphicViewer *viewer=nullptr);

                void set_current_speed(float adv, float rot);
                void set_current_advance_speed(float adv);
                void set_current_rot_speed(float rot);
                void set_current_pan_angle(float pan);

                void set_adv_tychon(float adv);
                void set_rot_tychon(float rot);

                void set_has_target(bool val);
                bool has_target() const;
                void set_desired_distance_to_target(float dist); //mm

                const float width = 250;
                const float length = 250;
                const float semi_width = width / 2;
                const float semi_height = length / 2;
                const float top_camera_height = 1555; //mm
                const float top_camera_y_offset = -40; //mm#include <abstract_graphic_viewer/abstract_graphic_viewer.h>
                float max_pan_angle = M_PI_2; // rad
                float min_pan_angle = -M_PI_2; // rad
                float max_advance_speed = 1500;
                float max_rot_speed = 1.5;

            private:
                float current_adv_speed = 0;
                float current_rot_speed = 0;
                float camera_pan_angle = 0.f;
                float camera_tilt_angle = 0.f;
                float min_distance_to_target = 50.f;
                Eigen::Transform<float, 3, Eigen::Affine> tf;
                std::vector<std::string> axes;
                RoboCompJointMotorSimple::JointMotorSimplePrxPtr joint_motor_proxy;
                RoboCompOmniRobot::OmniRobotPrxPtr omnirobot_proxy;
                //RoboCompYoloObjects::TBox current_target{.type = -1};
                target current_target;
                bool has_target_flag = false;
                float adv_tychon = 0.0f;
                float rot_tychon = 0.0f;
                std::map<float, float> bumper;
                Eigen::ArrayXf sector1, sector2, sector3,  sector4, sector5;
                void recompute_bumper(float dynamic_offset);
                // Dynamic window
                Dynamic_Window dynamic_window;

    };
} // rc

#endif //FORCEFIELD_ROBOT_H
