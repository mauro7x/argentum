#ifndef __CONFIG_H__
#define __CONFIG_H__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL2/SDL.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de la ventana

#define WINDOW_TITLE "Ventana creada con SDL"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 600
#define WINDOW_POS_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y SDL_WINDOWPOS_UNDEFINED
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de la cámara

#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 450
#define SCREEN_X_OFFSET 0
#define SCREEN_Y_OFFSET 150
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración del frame-rate

#define VSYNC_ENABLED true
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif  // __CONFIG_H__
