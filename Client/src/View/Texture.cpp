#include "../../includes/View/Texture.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Texture::Texture(SDL_Renderer* renderer)
    : texture(NULL), g_renderer(renderer), width(0), height(0) {}

Texture::Texture(Texture&& other) {
    this->texture = std::move(other.texture);
    this->g_renderer = std::move(other.g_renderer);
    this->width = std::move(other.width);
    this->height = std::move(other.height);
    other.texture = NULL;
    other.g_renderer = NULL;
    other.width = 0;
    other.height = 0;
}

Texture& Texture::operator=(Texture&& other) {
    this->texture = std::move(other.texture);
    this->g_renderer = std::move(other.g_renderer);
    this->width = std::move(other.width);
    this->height = std::move(other.height);
    other.texture = NULL;
    other.g_renderer = NULL;
    other.width = 0;
    other.height = 0;
    return *this;
}

void Texture::loadFromFile(std::string filepath, int r, int g, int b) {
    // Eliminamos una textura previa si existe
    free();

    SDL_Texture* new_texture = NULL;

    SDL_Surface* loaded_surface = IMG_Load(filepath.c_str());
    if (loaded_surface == NULL) {
        throw SDLException("Error in function IMG_Load()\nSDL_Error: %s",
                           SDL_GetError());
    }

    // Color key image (transparentar el fondo)
    if (r >= 0 && g >= 0 && b >= 0) {
        if (SDL_SetColorKey(loaded_surface, SDL_TRUE,
                            SDL_MapRGB(loaded_surface->format, r, g, b))) {
            throw SDLException(
                "Error in function SDL_SetColorKey()\nSDL_Error: %s",
                SDL_GetError());
        }
    }

    // Creamos la textura desde la surface
    new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
    if (new_texture == NULL) {
        throw SDLException(
            "Error in function SDL_CreateTextureFromSurface()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Dimensiones
    width = loaded_surface->w;
    height = loaded_surface->h;

    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);

    texture = new_texture;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) const {
    if (SDL_SetTextureColorMod(texture, red, green, blue)) {
        fprintf(stderr, "SDL Warning: Color modulation error!\n");
    }
}

void Texture::setBlendMode(SDL_BlendMode blending) const {
    if (SDL_SetTextureBlendMode(texture, blending)) {
        fprintf(stderr, "SDL Warning: Blend mode error!\n");
    }
}

void Texture::setAlpha(Uint8 alpha) const {
    if (SDL_SetTextureAlphaMod(texture, alpha)) {
        fprintf(stderr, "SDL Warning: Alpha modulation error!\n");
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, SDL_Rect* scale,
                     double angle, SDL_Point* center,
                     SDL_RendererFlip flip) const {
    // Set rendering space and render to screen
    SDL_Rect render_quad = {x, y, width, height};

    // Scaling /* usar scaling y clip al mismo tiempo se buggea, ojo */
    if (scale) {
        render_quad.w = scale->w;
        render_quad.h = scale->h;
    }

    // Set clip rendering dimensions
    if (clip) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    // Render to screen
    if (SDL_RenderCopyEx(g_renderer, texture, clip, &render_quad, angle, center,
                         flip)) {
        throw SDLException(
            "Error in function SDL_RenderCopyEx()\nSDL_Error: %s",
            SDL_GetError());
    }
}

void Texture::free() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

Texture::~Texture() {
    free();
}

//-----------------------------------------------------------------------------
