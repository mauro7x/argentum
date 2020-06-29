#ifndef __BUTTON_H__
#define __BUTTON_H__

//-----------------------------------------------------------------------------
#include "../Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Button {
    // Flag de animación
    bool mouse_over;

    // Offset de renderizado
    SDL_Rect render_box;

    // Texturas
    Texture base;
    Texture pressed;

    // Constructor por defecto
    Button() : mouse_over(false), render_box({0}) {}
};

//-----------------------------------------------------------------------------

#endif  // __BUTTON_H__