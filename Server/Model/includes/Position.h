#ifndef __POSITION_H__
#define __POSITION_H__

#include "../../../Common/includes/MapContainer.h"

#ifndef __ORIENTATION__
#define __ORIENTATION__
enum Orientation { UP, DOWN, LEFT, RIGHT };
#endif // __ORIENTATION__

class Position {
    private:
        int id_map;
        int x, y;
        Orientation orientation;
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

        const bool move(const Orientation& orientation);
};

#endif