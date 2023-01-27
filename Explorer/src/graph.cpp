#include "graph.h"

Graph::Graph()
{
    printf("Graph created");
}

//DESTRUCTOR
Graph::~Graph()
{
    printf("Graph destroyed");
}

void Graph::add_room(Room &room)
{
    room.id = rooms.size();
    rooms.push_back(room);
}

void Graph::add_link(std::pair<int, int> link)
{
    if (links.find(link) == links.end())
    {
        links.insert(link);
        printf("Added link between %d and %d\n", link.first, link.second);
    }   
}

void Graph::add_item_to_room(Room &room, int item_id, Eigen::Vector3f coords)
{
    room.items.push_back(std::pair(item_id, Eigen::Vector2f(coords.x(), coords.y())));
}

void Graph::add_item_to_room(int room_id, int item_id, Eigen::Vector3f coords)
{
    for (auto &room : rooms) 
        if (room.id == room_id) 
            room.items.push_back(std::pair(item_id, Eigen::Vector2f(coords.x(), coords.y())));
}

void Graph::update_room(int room_id, Room &room)
{
    for (auto &r : rooms) 
        if (r.id == room_id) 
            r = room;
}

//Compare rooms by their items, to compare them, check if the ID is the same and the distance between the coords is less than 200
bool Graph::compare_rooms(Room &room1, Room &room2)
{

    auto compare_items = [](auto &item1, auto &item2){
        if (item1.first == item2.first)
            if ((item1.second - item2.second).norm() < 200) 
                return true;
        return false;
    };

    if (room1.items.size() != room2.items.size())
        return false;

    bool global = true;
    for (auto &item1 : room1.items) {
        bool local = false;
        for (auto &item2 : room2.items) {
            local = local || compare_items(item1, item2);
        }

        global = global && local;
    }

    return global;
}

int Graph::find_room(Room &room)
{
    for (auto &r : rooms) {
        if (compare_rooms(r, room)) {
            printf("Entering existing room, ID: %d\n", r.id);
            return r.id;
        }
    }
    //append the room to the graph
    add_room(room);
    printf("Found new room, ID: %lu\n", rooms.size() -1);
    return rooms.size() - 1;
}



