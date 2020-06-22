#ifndef __USER_INVENTORY_H__
#define __USER_INVENTORY_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/types.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUDComponent.h"
#include "ItemSpriteContainer.h"
#include "Player.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UserInventory : public HUDComponent {
   private:
    const ItemSpriteContainer& g_item_sprites;
    const Player& g_player;

    // Offsets de renderizado
    int slot_w = 0;
    int slot_h = 0;
    SDL_Point equipment_weapon = {0};
    SDL_Point equipment_helmet = {0};
    SDL_Point equipment_armour = {0};
    SDL_Point equipment_shield = {0};

    // Texturas a renderizar
    Texture base;
    EquipmentData equipment;

    /* Renderiza el equipamiento del jugador */
    void _renderEquipment() const;

   public:
    /* Constructor */
    UserInventory(const Renderer* renderer,
                  const ItemSpriteContainer& item_sprites,
                  const Player& player);

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config) override;

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Actualiza la información que se muestra */
    void update() override;

    /* Renderiza la consola */
    void render() const override;

    /* Libera recursos */
    virtual void free() override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UserInventory();
};

//-----------------------------------------------------------------------------

#endif  // __USER_INVENTORY_H__
