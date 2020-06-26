#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <array>
#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/paths.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Texture.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define PLAYER_NICKNAME_COLOR SDL_Color({255, 191, 0, 0})
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : public Unit {
   private:
    // Datos del jugador
    std::string nickname;
    Id head_id = 0, body_id = 0;
    EquipmentData equipment = {0};
    InventoryData inventory = {0};

    uint32_t health = 0, max_health = 0;
    uint32_t mana = 0, max_mana = 0;
    uint32_t safe_gold = 0, excess_gold = 0;
    uint32_t level = 0, exp = 0, levelup_exp = 0;

    // Texturas para el nickname
    TTF_Font* nickname_font = NULL;
    TTF_Font* level_font = NULL;
    Texture info_nickname;
    Texture info_level;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Copia la data desde el paquete recibido */
    void _copyData(const PlayerData& init_data);

    /* Renderiza la información del personaje */
    void _renderInfo() const;

    //-------------------------------------------------------------------------

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

    /* Carga media necesaria */
    void loadMedia();

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    /* Libera recursos */
    void free();

    /* Obtiene la posición y dimensiones en pixeles */
    SDL_Rect getBox() const;

    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Getters para graficar en el HUD

    const std::string& getNickname() const;

    const EquipmentData& getEquipment() const;
    const InventoryData& getInventory() const;

    const uint32_t& getHealth() const;
    const uint32_t& getMaxHealth() const;

    const uint32_t& getMana() const;
    const uint32_t& getMaxMana() const;

    const uint32_t& getSafeGold() const;
    const uint32_t& getExcessGold() const;

    const uint32_t& getLevel() const;
    const uint32_t& getExp() const;
    const uint32_t& getLvlUpExp() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
