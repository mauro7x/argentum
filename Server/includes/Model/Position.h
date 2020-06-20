#ifndef __POSITION_H__
#define __POSITION_H__

#include <exception>

#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"

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

        /*
         * Devuelve la distancia Manhhattan [1] entre las coordenadas
         * de this y las de other.
         */
        const unsigned int getDistance(const Position& other) const;

        /* Devuelve la coordenada x de la posición del character */
        const int getX() const;

        /* Devuelve la coordenada y de la posición del character */
        const int getY() const;

        /* Devuelve la orientación de la posición del character */
        const Orientation getOrientation() const;

        /* 
         * Mueve un tile la posición en la dirección de la orientation. 
         * 
         * Lanza CollisionWhileMovingException si no se puede mover
         * a causa de una colisión.
         */
        void move(const Orientation& orientation);
};

class CollisionWhileMovingException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif
