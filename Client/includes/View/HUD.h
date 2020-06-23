#ifndef __HUD_H__
#define __HUD_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "Console.h"
#include "ItemSpriteContainer.h"
#include "Player.h"
#include "Renderer.h"
#include "Texture.h"
#include "UserInfo.h"
#include "UserInventory.h"
#include "UserStats.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HUD {
   private:
    bool initialized;
    const Renderer* g_renderer;
    const ItemSpriteContainer& g_item_sprites;
    const Player& g_player;

    // Offsets de renderizado
    int offset_x = 0, offset_y = 0;
    int w = 0, h = 0;

    // Componentes
    Console console;
    UserInfo user_info;
    UserInventory user_inventory;
    UserStats user_stats;

    // Capa frontal de decoración
    Texture upper_layer;

   public:
    /* Constructor */
    HUD(const Renderer* renderer, const ItemSpriteContainer& item_sprites,
        const Player& player);

    /* Deshabilitamos el constructor por copia. */
    HUD(const HUD&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    HUD& operator=(const HUD&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    HUD(HUD&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    HUD& operator=(HUD&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config);

    /* Carga los archivos necesarios */
    void loadMedia();

    //-------------------------------------------------------------------------
    // Interacción con la consola

    /* Habilita la consola */
    void enableInput();

    /* Escribe el texto recibido a la consola */
    void newInputText(const char* text);

    /* Borra el último caracter escrito */
    void removeChar();

    /* Retorna el texto escrito y limpia la consola */
    std::string popText();

    /* Deshabilita la consola */
    void disableInput();

    //-------------------------------------------------------------------------

    /* Actualiza su información */
    void update();

    /* Renderiza la interfaz entera */
    void render() const;

    /* Libera recursos */
    void free();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~HUD();
};

//-----------------------------------------------------------------------------

#endif  // __HUD_H__
