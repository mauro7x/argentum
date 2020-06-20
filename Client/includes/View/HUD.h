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
    const Player& player;

    // Offsets de renderizado
    int offset_x, offset_y;
    int w, h;

    // Componentes
    Console console;
    UserInfo user_info;
    UserInventory user_inventory;
    UserStats user_stats;

    // Capa frontal de decoración
    Texture upper_layer;

   public:
    /* Constructor */
    HUD(const Renderer* renderer, const Player& player);

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

    /* Renderiza la interfaz entera */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~HUD();
};

//-----------------------------------------------------------------------------

#endif  // __HUD_H__
