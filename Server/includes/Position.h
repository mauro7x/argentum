#ifndef __POSITION_H__
#define __POSITION_H__

#include "../../Common/includes/MapContainer.h"

class Position {
    private:
        int id_map;
        int x, y;
        MapContainer& map_container;
    
    public:
        Position(const int id_map, 
                 const int init_x_coord,
                 const int init_y_coord,
                 MapContainer& map_container);
        ~Position();

        Position(const Position&) = delete;
        Position& operator=(const Position&) = delete;
        Position(Position&&) = delete;
        Position& operator=(Position&&) = delete;

        const unsigned int getDistance(const Position& other) const;

        const int getX() const;
        const int getY() const;
};

#endif