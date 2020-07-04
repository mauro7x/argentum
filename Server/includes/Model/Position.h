#ifndef __POSITION_H__
#define __POSITION_H__

#include <exception>

#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
#include "config_structs.h"

class Position {
   private:
    Id map;
    int x, y;
    Orientation orientation;
    MapContainer& map_container;

   public:
    Position(const Id map, const int init_x_coord, const int init_y_coord,
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

    const unsigned int getDistance(const int other_x, const int other_y) const;

    /*
     * Devuelve el maximo valor entre las coordenadas de this y
     *las de other
     */
    const unsigned int getRange(const Position& other) const;

    /* Devuelve la coordenada x de la posición del character */
    const int getX() const;

    /* Devuelve la coordenada y de la posición del character */
    const int getY() const;

    /* Devuelve la orientacion del character */
    const Orientation getOrientation() const;

    /* Devuelve el Id del mapa en el que se encuentra */
    const Id getMapId() const;

    /* Cambia la orientacion por la especificada */
    void changeOrientation(Orientation orientation);

    /* 
     * Cambia el tile actual al tile libre más cercano a las coordenadas pasadas
     * por parámetro. 
     * 
     * Si no se encuentra tile libre, no hace nada.
     */
    void changePosition(int x, int y);

    /*
     * Mueve un tile la posición en la dirección de la orientation.
     *
     * Con parametro si es criatura para verificar.
     * Lanza CollisionWhileMovingException si no se puede mover
     * a causa de una colisión.
     */
    void move(bool is_creature);


    const bool isInSafeZone() const;

    /*
     * Llena los campos x_tile, y_tile y orientation del
     * PlayerData para su broadcast.
     */
    void fillBroadcastData(UnitData& data) const;

    /*
     * Llena los campos x_tile, y_tile del CharacterCfg.
     */
    void fillPersistenceData(CharacterCfg& data) const;
};

class CollisionWhileMovingException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif
