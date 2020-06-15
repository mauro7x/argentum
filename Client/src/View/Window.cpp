#include "../../includes/View/Window.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Window::_fullscreenMode() {
    SDL_SetWindowFullscreen(window, SDL_TRUE);
    fullscreen = true;
}

void Window::_windowMode() {
    SDL_SetWindowFullscreen(window, SDL_FALSE);
    fullscreen = false;
}

void Window::_free() {
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Window::Window()
    : initialized(false),
      window(NULL),
      fullscreen_allowed(false),
      fullscreen(false),
      w(0),
      h(0) {}

void Window::init(const json& config) {
    std::unique_lock<std::mutex> l(m);
    if (initialized) {
        throw Exception("Window already initialized.");
    }

    fullscreen_allowed = config["fullscreen"];
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

    if (fullscreen_allowed) {
        _fullscreenMode();
    }

    initialized = true;
}

void Window::fullscreenModeSwitch() {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Window not initialized.");
    }

    if (fullscreen_allowed) {
        if (fullscreen) {
            _windowMode();
        } else {
            _fullscreenMode();
        }
    }
}

SDL_Window* Window::getWindow() {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Window not initialized.");
    }

    return window;
}

Window::~Window() {
    _free();
}

//-----------------------------------------------------------------------------
