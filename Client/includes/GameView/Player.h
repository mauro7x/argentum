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
#include "../SDL/Texture.h"
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define PLAYER_NICKNAME_COLOR SDL_Color({255, 191, 0, 255})
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
    Texture info_nickname_shadow;
    Texture info_level;

    // Si es enano/gnomo usamos sus sprites
    bool is_shorter = false;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Copia la data desde el paquete recibido */
    void _copyData(const PlayerData& data);

    /* Renderiza la información del personaje */
    void _renderInfo() const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Player(const Renderer* renderer, const Camera& camera,
           UnitSpriteContainer* sprites);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Player(Player&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Player& operator=(Player&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga media necesaria (debe llamarse antes de a init) */
    void loadMedia();

    /* Se inicializa con el primer paquete del server */
    void init(const PlayerData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

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
