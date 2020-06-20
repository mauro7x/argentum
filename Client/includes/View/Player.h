#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <array>
#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/paths.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : public Unit {
   private:
    // Cuerpo y vestimenta
    Id head_id = 0, body_id = 0;
    Id helmet_id = 0, armour_id = 0, shield_id = 0, weapon_id = 0;

    // Vida, mana, oro, nivel
    uint32_t health = 0, max_health = 0;
    uint32_t mana = 0, max_mana = 0;
    uint32_t safe_gold = 0, excess_gold = 0;
    uint32_t level = 0, exp = 0, levelup_exp = 0;

    // Inventario y equipamiento
    InventoryData inventory = {0};
    EquipmentData equipment = {0};

    /* Copia la data desde el paquete recibido */
    void _copyData(const PlayerData& init_data);

   public:
    /* Constructor */
    Player(Renderer* renderer, UnitSpriteContainer* sprites);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Player(Player&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Player& operator=(Player&& other) = delete;

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const PlayerData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    /* Obtiene la posición en tiles */
    SDL_Point getPos() const;

    /* Obtiene la posición y dimensiones en pixeles */
    SDL_Rect getBox() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
