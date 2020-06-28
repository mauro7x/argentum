#include "../../includes/SDL/Window.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Window::Window() {}

void Window::init(bool fullscreen, int w, int h, const char* title) {
    if (initialized) {
        throw Exception("Window already initialized.");
    }

    this->w = w;
    this->h = h;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw Exception("Error in function SDL_CreateWindow()\nSDL_Error: %s",
                        SDL_GetError());
    }

    if (fullscreen) {
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    }

    initialized = true;
}

SDL_Window* Window::getWindow() {
    if (!initialized) {
        throw Exception("Window not initialized.");
    }

    return window;
}

Window::~Window() {
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

//-----------------------------------------------------------------------------
