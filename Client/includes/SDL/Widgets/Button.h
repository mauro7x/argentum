#ifndef __BUTTON_H__
#define __BUTTON_H__

//-----------------------------------------------------------------------------
#include "../Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Button {
    bool mouse_over;
    SDL_Rect render_box;
    Texture base;
    Texture pressed;

    // Constructor por defecto
    Button() : mouse_over(false) {}
};

//-----------------------------------------------------------------------------

#endif  // __BUTTON_H__