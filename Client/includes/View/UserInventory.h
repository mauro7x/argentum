#ifndef __USER_INVENTORY_H__
#define __USER_INVENTORY_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <array>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Inventory.h"
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
    SDL_Point first_inventory_slot = {0};
    int inventory_rows = 0;
    int slots_per_row = 0;
    SDL_Point offset_between_slots = {0};
    SDL_Rect safe_gold_box = {0};
    SDL_Point safe_gold_pos = {0};
    SDL_Rect excess_gold_box = {0};
    SDL_Point excess_gold_pos = {0};

    // Fuentes a utilizar
    int quantities_fontsize = 0;
    TTF_Font* quantities_font = NULL;
    int gold_fontsize = 0;
    TTF_Font* gold_font = NULL;

    // Texturas a renderizar
    Texture base;
    EquipmentData equipment = {0};
    InventoryData inventory = {0};
    Texture safe_gold, excess_gold;

    // Valores cambiantes
    std::array<Texture, N_INVENTORY_SLOTS> inventory_quantities;
    int current_safe_gold = 0;
    int current_excess_gold = 0;

    /* Centra una textura en un rectangulo */
    void _center(SDL_Point& texture_pos, const Texture& texture,
                 const SDL_Rect& rect);

    /* Renderiza el equipamiento del jugador */
    void _renderEquipment() const;

    /* Renderiza el inventario del jugador */
    void _renderInventory() const;

    /* Renderiza el oro del jugador */
    void _renderGold() const;

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
