#ifndef __SELECTION_INPUT_BOX_H__
#define __SELECTION_INPUT_BOX_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct SelectionInputBox {
    // Flags de animación
    bool prev_over;
    bool next_over;

    // Offsets de renderizado
    SDL_Rect prev_box;
    SDL_Rect next_box;
    SDL_Rect bar_box;

    // Texturas
    Texture prev;
    Texture prev_pressed;
    Texture next;
    Texture next_pressed;
    Texture bar;

    // Constructor por defecto
    SelectionInputBox()
        : prev_over(false),
          next_over(false),
          prev_box({0}),
          next_box({0}),
          bar_box({0}) {}
};

//-----------------------------------------------------------------------------

#endif  // __SELECTION_INPUT_BOX_H__