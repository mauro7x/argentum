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

void Renderer::_resize(SDL_Rect* render_quad) const {
    render_quad->x = std::floor(render_quad->x * scale_factor_w);
    render_quad->y = std::floor(render_quad->y * scale_factor_h);
    render_quad->w = std::ceil(render_quad->w * scale_factor_w);
    render_quad->h = std::ceil(render_quad->h * scale_factor_h);
}

void Renderer::_free() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Renderer::Renderer(Window& window, const Camera& camera)
    : initialized(false), window(window), camera(camera), renderer(NULL) {}

void Renderer::init(const json& config, const float scale_factor_w,
                    const float scale_factor_h) {
    if (initialized) {
        throw Exception("Renderer already initialized.");
    }

    this->scale_factor_w = scale_factor_w;
    this->scale_factor_h = scale_factor_h;
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

    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
        throw SDLException(
            "Error in function SDL_SetRenderDrawBlendMode()\nSDL_Error: %s",
            SDL_GetError());
    }

    _setDrawColor();

    initialized = true;
}

void Renderer::clearScreen() const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    if (SDL_RenderClear(renderer)) {
        throw SDLException("Error in function SDL_RenderClear()\nSDL_Error: %s",
                           SDL_GetError());
    }
}

void Renderer::presentScreen() const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface) const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (new_texture == NULL) {
        throw SDLException(
            "Error in function SDL_CreateTextureFromSurface()\nSDL_Error: %s",
            SDL_GetError());
    }
    return new_texture;
}

void Renderer::render(SDL_Texture* texture, SDL_Rect* render_quad,
                      const SDL_Rect* clip) const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    /* Escalamos a las dimensiones en las que estamos renderizando */
    _resize(render_quad);

    if (SDL_RenderCopy(renderer, texture, clip, render_quad)) {
        throw SDLException(
            "Error in function SDL_RenderCopyEx()\nSDL_Error: %s",
            SDL_GetError());
    }
}

void Renderer::renderIfVisible(SDL_Texture* texture, SDL_Rect* render_quad,
                               const SDL_Rect* clip) const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    if (camera.isVisible(render_quad)) {
        render_quad->x += camera.xOffset();
        render_quad->y += camera.yOffset();
        render(texture, render_quad, clip);
    }
}

/* Rellena el rectangulo */
void Renderer::fillQuad(SDL_Rect* quad, int r, int g, int b, int a) const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    if (SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
        throw SDLException(
            "Error in function SDL_SetRenderDrawColor() while filling "
            "quad.\nSDL_Error: %s",
            SDL_GetError());
    }

    /* Escalamos a las dimensiones en las que estamos renderizando */
    _resize(quad);

    SDL_RenderFillRect(renderer, quad);
}

void Renderer::fillQuadIfVisible(SDL_Rect* quad, int r, int g, int b,
                                 int a) const {
    if (!initialized) {
        throw Exception("Renderer not initialized.");
    }

    if (camera.isVisible(quad)) {
        quad->x += camera.xOffset();
        quad->y += camera.yOffset();
        fillQuad(quad, r, g, b, a);
    }
}

Renderer::~Renderer() {
    _free();
}

//-----------------------------------------------------------------------------
