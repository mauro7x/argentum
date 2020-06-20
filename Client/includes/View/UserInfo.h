#ifndef __USER_INFO_H__
#define __USER_INFO_H__

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
#include "Player.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UserInfo : public HUDComponent {
   private:
    const Player& player;

    // Texturas a renderizar
    Texture base;

   public:
    /* Constructor */
    UserInfo(const Renderer* renderer, const Player& player);

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config) override;

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Renderiza la consola */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UserInfo();
};

//-----------------------------------------------------------------------------

#endif  // __USER_INFO_H__
