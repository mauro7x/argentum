#ifndef __USER_STATS_H__
#define __USER_STATS_H__

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
#include "ProgressBar.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UserStats : public HUDComponent {
   private:
    const Player& player;

    // Texturas a renderizar
    Texture base;

    ProgressBar health;
    ProgressBar mana;

   public:
    /* Constructor */
    UserStats(const Renderer* renderer, const Player& player);

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
    ~UserStats();
};

//-----------------------------------------------------------------------------

#endif  // __USER_STATS_H__
