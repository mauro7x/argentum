#ifndef __POSITION_H__
#define __POSITION_H__

#include <exception>

#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/types.h"

class Position {
    private:
        Id map;
        int x, y;
        Orientation orientation;
        MapContainer& map_container;
    
    public:
        Position(const Id map, 
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

        /* 
         * Mueve un tile la posición en la dirección de la orientation. 
         * 
         * Lanza CollisionWhileMovingException si no se puede mover
         * a causa de una colisión.
         */
        void move(const Orientation& orientation);

        /*
         * Llena los campos x_tile, y_tile y orientation del
         * PlayerData para su broadcast.
         */
        void fillBroadcastData(PlayerData& data) const;
};

class CollisionWhileMovingException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif
