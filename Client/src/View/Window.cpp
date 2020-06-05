#include "../../includes/View/Window.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Window::Window() : window(NULL) {}

void Window::init() {
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_POS_X, WINDOW_POS_Y,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw SDLException(
            "Error in function SDL_CreateWindow()\nSDL_Error: %s",
            SDL_GetError());
    }
}

SDL_Window* Window::getWindow() const {
    return window;
}

void Window::free() {
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

Window::~Window() {
    free();
}

//-----------------------------------------------------------------------------
