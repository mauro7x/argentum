#include "../../includes/View/Window.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Window::_free() {
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Window::Window() : window(NULL) {}

void Window::init(const json& config) {
    w = config["w"];
    h = config["h"];
    std::string title = config["title"];

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw SDLException(
            "Error in function SDL_CreateWindow()\nSDL_Error: %s",
            SDL_GetError());
    }
}

SDL_Window* Window::getWindow() const {
    return window;
}

Window::~Window() {
    _free();
}

//-----------------------------------------------------------------------------
