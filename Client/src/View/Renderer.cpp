#include "../../includes/View/Renderer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Renderer::_setDrawColor() const {
    if (SDL_SetRenderDrawColor(renderer, draw_color_r, draw_color_g,
                               draw_color_b, draw_color_a)) {
        throw SDLException(
            "Error in function SDL_SetRenderDrawColor()\nSDL_Error: %s",
            SDL_GetError());
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Renderer::Renderer(const Window& window, const Camera& camera)
    : window(window), camera(camera), renderer(NULL) {}

void Renderer::init(const json config) {
    draw_color_r = config["draw_color"]["r"];
    draw_color_g = config["draw_color"]["g"];
    draw_color_b = config["draw_color"]["b"];
    draw_color_a = config["draw_color"]["a"];
    bool vsync = config["vsync"];

    Uint32 renderer_flags =
        vsync ? (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
              : (SDL_RENDERER_ACCELERATED);

    renderer = SDL_CreateRenderer(window.getWindow(), -1, renderer_flags);
    if (renderer == NULL) {
        throw SDLException(
            "Error in function SDL_CreateRenderer()\nSDL_Error: %s",
            SDL_GetError());
    }

    _setDrawColor();
}

void Renderer::clearScreen() const {
    if (SDL_RenderClear(renderer)) {
        throw SDLException("Error in function SDL_RenderClear()\nSDL_Error: %s",
                           SDL_GetError());
    }
}

void Renderer::presentScreen() const {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface) const {
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (new_texture == NULL) {
        throw SDLException(
            "Error in function SDL_CreateTextureFromSurface()\nSDL_Error: %s",
            SDL_GetError());
    }
    return new_texture;
}

void Renderer::render(SDL_Texture* texture, SDL_Rect* render_quad,
                      const SDL_Rect* clip, double angle, SDL_Point* center,
                      SDL_RendererFlip flip) const {
    // Set clip rendering dimensions
    if (clip) {
        render_quad->w = clip->w;
        render_quad->h = clip->h;
    }

    // Render to screen
    if (SDL_RenderCopyEx(renderer, texture, clip, render_quad, angle, center,
                         flip)) {
        throw SDLException(
            "Error in function SDL_RenderCopyEx()\nSDL_Error: %s",
            SDL_GetError());
    }
}

void Renderer::renderIfVisible(SDL_Texture* texture, SDL_Rect* render_quad,
                               const SDL_Rect* clip, double angle,
                               SDL_Point* center, SDL_RendererFlip flip) const {
    if (camera.isVisible(render_quad)) {
        render_quad->x += camera.xOffset();
        render_quad->y += camera.yOffset();
        render(texture, render_quad, clip, angle, center, flip);
    }
}

void Renderer::free() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
}

Renderer::~Renderer() {
    free();
}

//-----------------------------------------------------------------------------
