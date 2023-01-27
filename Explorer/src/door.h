#ifndef DOOR_H
#define DOOR_H

#include <Eigen/Dense>

class Door
{
    private:
        //identificadores de las habitaciones con las que se comunica en una tupla de dos ints
        std::tuple<int, int> rooms;
        //coordenadas del centro de la puerta
        Eigen::Vector3f coords;
    public:
        Door();
        Door(std::tuple<int, int> rooms, Eigen::Vector3f coords);
        std::tuple<int, int> get_rooms() const;
        Eigen::Vector3f get_coords() const;
        void set_rooms(std::tuple<int, int> rooms);
};

#endif