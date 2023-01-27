#ifndef GRAPH_H
#define GRAPH_H

#include <Eigen/Dense>
#include <vector>
#include <tuple>
#include <set>

class Graph
{
    public:
        struct Room
        {
            //self id
            int id;
            //vector with pairs of ids and coordinates
            std::vector<std::pair<int, Eigen::Vector2f>> items;
        };
        Graph();
        ~Graph();
        void add_room(Room &room);
        void add_link(std::pair<int, int> link);
        void add_item_to_room(Room &room, int item_id, Eigen::Vector3f coords);
        void add_item_to_room(int room_id, int item_id, Eigen::Vector3f coords) ;
        void update_room(int room_id, Room &room);
        bool compare_rooms(Room &room1, Room &room2);
        int find_room(Room &room);

    private:
        std::vector<Room> rooms;
        // because we want an structure with no duplicat
        std::set<std::pair<int, int>> links;

};




#endif