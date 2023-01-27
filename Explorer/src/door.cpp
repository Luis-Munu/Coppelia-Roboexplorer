#include "door.h"

Door::Door()
{
    rooms = std::make_tuple(-1, -1);
    coords = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
}

Door::Door(std::tuple<int, int> rooms, Eigen::Vector3f coords)
{
    this->rooms = rooms;
    this->coords = coords;
}

std::tuple<int, int> Door::get_rooms() const
{
    return rooms;
}

Eigen::Vector3f Door::get_coords() const
{
    return coords;
}

void Door::set_rooms(std::tuple<int, int> rooms)
{
    this->rooms = rooms;
}

