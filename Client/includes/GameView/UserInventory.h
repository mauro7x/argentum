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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Renderer.h"
#include "../SDL/Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../paths.h"
#include "HUDComponent.h"
#include "ItemSpriteContainer.h"
#include "Player.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define EQUIPMENT_UNEQUIP_ANIMATION_ITS 10
//-----------------------------------------------------------------------------

class UserInventory : public HUDComponent {
   private:
    const ItemSpriteContainer& g_item_sprites;
    const Player& g_player;

    // Offsets de renderizado
    int slot_w = 0;
    int slot_h = 0;
    std::array<SDL_Point, N_INVENTORY_SLOTS> inventory_slots = {0};
    std::array<SDL_Point, N_WEARABLE_ITEMS> equipment_slots = {0};

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
    Texture selection;
    EquipmentData equipment = {0};
    InventoryData inventory = {0};
    Texture safe_gold, excess_gold;

    // Valores cambiantes
    std::array<Texture, N_INVENTORY_SLOTS> inventory_quantities;
    int current_safe_gold = 0;
    int current_excess_gold = 0;
    bool item_selected = false;
    SDL_Point item_selected_pos = {0};

    // Animación de dobleclick para el equipamiento
    int16_t current_animate_slot = 0;
    SDL_Point animate_slot_pos = {0};

    /* Renderiza el equipamiento del jugador */
    void _renderEquipment() const;

    /* Renderiza el inventario del jugador */
    void _renderInventory() const;

    /* Renderiza el oro del jugador */
    void _renderGold() const;

    /* Chequea si el slot fue clickeado */
    bool _wasClicked(const SDL_Point& click_pos,
                     const SDL_Point& slot_pos) const;

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

    /* Obtiene el slot del equipamiento que fue clickeado
     * (retorna -1 si ningún slot fue clickeado) */
    int8_t getEquipmentSlotClicked(const SDL_Point& click_pos);

    /* Obtiene el slot del inventario que fue clickeado
     * (retorna -1 si ningún slot fue clickeado) */
    int8_t getInventorySlotClicked(const SDL_Point& click_pos);

    /* Selecciona un item del inventario */
    void selectItem(uint8_t inventory_slot);

    /* Elimina la selección */
    void clearSelection();

    /* Actualiza la información que se muestra */
    void update(const int it) override;

    /* Renderiza la consola */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UserInventory();
};

//-----------------------------------------------------------------------------

#endif  // __USER_INVENTORY_H__
