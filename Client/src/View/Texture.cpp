#include "../../includes/View/Texture.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Texture::Texture() : texture(NULL), width(0), height(0) {}

Texture::Texture(Texture&& other) {
    this->texture = std::move(other.texture);
    this->width = std::move(other.width);
    this->height = std::move(other.height);
    other.texture = NULL;
    other.width = 0;
    other.height = 0;
}

Texture& Texture::operator=(Texture&& other) {
    this->texture = std::move(other.texture);
    this->width = std::move(other.width);
    this->height = std::move(other.height);
    other.texture = NULL;
    other.width = 0;
    other.height = 0;
    return *this;
}

void Texture::loadFromFile(const Renderer* renderer, std::string filepath,
                           int r, int g, int b) {
    // Eliminamos una textura previa si existe
    free();

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
    texture = renderer->createTextureFromSurface(loaded_surface);

    // Seteamos las dimensiones
    width = loaded_surface->w;
    height = loaded_surface->h;

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
            throw SDLException(
                "Error in function loadFromRenderedText(). Unknown text type.");
        }
    }

    if (text_surface == NULL) {
        throw SDLException(
            "Error in function TTF_RenderText_Solid()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Creamos la textura desde la surface
    texture = renderer->createTextureFromSurface(text_surface);

    // Seteamos las dimensiones
    width = text_surface->w;
    height = text_surface->h;

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
        throw SDLException(
            "Error in function TTF_RenderText_Solid()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Creamos la textura desde la surface
    texture = renderer->createTextureFromSurface(text_surface);

    // Seteamos las dimensiones
    width = text_surface->w;
    height = text_surface->h;

    // Liberamos la surface
    SDL_FreeSurface(text_surface);
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

SDL_Texture* Texture::getTexture() const {
    return texture;
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
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
