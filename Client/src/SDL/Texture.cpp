#include "../../includes/SDL/Texture.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Texture::Texture() {}

void Texture::loadFromFile(const Renderer* renderer, std::string filepath,
                           int r, int g, int b) {
    // Eliminamos una textura previa si existe
    free();

    SDL_Surface* loaded_surface = IMG_Load(filepath.c_str());
    if (loaded_surface == NULL) {
        throw Exception("Error in function IMG_Load()\nSDL_Error: %s",
                        SDL_GetError());
    }

    // Color key image (transparentar el fondo)
    if (r >= 0 && g >= 0 && b >= 0) {
        if (SDL_SetColorKey(loaded_surface, SDL_TRUE,
                            SDL_MapRGB(loaded_surface->format, r, g, b))) {
            throw Exception(
                "Error in function SDL_SetColorKey()\nSDL_Error: %s",
                SDL_GetError());
        }
    }

    // Creamos la textura desde la surface
    texture = renderer->createTextureFromSurface(loaded_surface);

    // Seteamos las dimensiones
    w = loaded_surface->w;
    h = loaded_surface->h;

    // Liberamos la surface
    SDL_FreeSurface(loaded_surface);
}

void Texture::loadFromRenderedText(const Renderer* renderer, TTF_Font* font,
                                   const std::string& text, SDL_Color color,
                                   TextType type) {
    // Eliminamos una textura previa si existe
    free();

    // Generamos la surface de texto
    SDL_Surface* text_surface = NULL;

    switch (type) {
        case SOLID_TEXT: {
            text_surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
            break;
        }

        case BLENDED_TEXT: {
            text_surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
            break;
        }

        default: {
            throw Exception(
                "Error in function loadFromRenderedText(). Unknown text type.");
        }
    }

    if (text_surface == NULL) {
        throw Exception(
            "Error in function TTF_RenderText_Solid()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Creamos la textura desde la surface
    texture = renderer->createTextureFromSurface(text_surface);

    // Seteamos las dimensiones
    w = text_surface->w;
    h = text_surface->h;

    // Liberamos la surface
    SDL_FreeSurface(text_surface);
}

void Texture::loadFromRenderedWrappedText(const Renderer* renderer,
                                          TTF_Font* font,
                                          const std::string& text,
                                          int max_width, SDL_Color color) {
    // Eliminamos una textura previa si existe
    free();

    // Generamos la surface de texto
    SDL_Surface* text_surface =
        TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, max_width);

    if (text_surface == NULL) {
        throw Exception(
            "Error in function TTF_RenderText_Solid()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Creamos la textura desde la surface
    texture = renderer->createTextureFromSurface(text_surface);

    // Seteamos las dimensiones
    w = text_surface->w;
    h = text_surface->h;

    // Liberamos la surface
    SDL_FreeSurface(text_surface);
}

SDL_Texture* Texture::getTexture() const {
    if (!texture) {
        throw Exception("Texture::getTexture: texture is NULL.");
    }

    return texture;
}

int Texture::getWidth() const {
    if (!texture) {
        throw Exception("Texture::getTexture: texture is NULL.");
    }

    return w;
}

int Texture::getHeight() const {
    if (!texture) {
        throw Exception("Texture::getTexture: texture is NULL.");
    }

    return h;
}

void Texture::free() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        w = 0;
        h = 0;
    }
}

Texture::~Texture() {
    free();
}

//-----------------------------------------------------------------------------
