#ifndef __USER_INFO_H__
#define __USER_INFO_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cstdint>
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

    // Offsets de renderizado
    SDL_Rect nickname_box = {0};
    SDL_Point nickname_pos = {0};
    SDL_Rect lvl_box = {0};
    SDL_Point lvl_pos = {0};

    // Fuentes a utilizar
    TTF_Font* nickname_font;
    int nickname_fontsize = 0;
    TTF_Font* lvl_font;
    int lvl_fontsize = 0;

    // Texturas a renderizar
    Texture base;

    Texture nickname;
    std::string current_nickname;

    Texture lvl;
    uint32_t current_lvl = 0;

   public:
    /* Constructor */
    UserInfo(const Renderer* renderer, const Player& player);

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
    ~UserInfo();
};

//-----------------------------------------------------------------------------

#endif  // __USER_INFO_H__
