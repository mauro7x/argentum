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
#include "../paths.h"
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

    // Texturas a renderizar
    Texture base;

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
