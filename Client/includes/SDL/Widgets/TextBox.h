#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct TextBox {
    // Flag de animación
    bool is_active;

    // Offsets de renderizado
    SDL_Rect render_box;
    SDL_Point render_pos;

    // Texto contenido
    std::string s_content;

    // Texturas
    Texture base;
    Texture active;
    Texture content;

    // Constructor por defecto
    TextBox() : is_active(false), render_box({0}), render_pos({0}) {}
};

//-----------------------------------------------------------------------------

#endif  // __TEXTBOX_H__