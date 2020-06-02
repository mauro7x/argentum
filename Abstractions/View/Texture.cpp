#include "Texture.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Texture::Texture(SDL_Renderer* renderer)
    : mTexture(NULL), mRenderer(renderer), mWidth(0), mHeight(0) {}

Texture::Texture(Texture&& other) {
    this->mTexture = std::move(other.mTexture);
    this->mRenderer = std::move(other.mRenderer);
    this->mWidth = std::move(other.mWidth);
    this->mHeight = std::move(other.mHeight);
    other.mTexture = NULL;
    other.mRenderer = NULL;
    other.mWidth = 0;
    other.mHeight = 0;
}

Texture& Texture::operator=(Texture&& other) {
    this->mTexture = std::move(other.mTexture);
    this->mRenderer = std::move(other.mRenderer);
    this->mWidth = std::move(other.mWidth);
    this->mHeight = std::move(other.mHeight);
    other.mTexture = NULL;
    other.mRenderer = NULL;
    other.mWidth = 0;
    other.mHeight = 0;
    return *this;
}

void Texture::loadFromFile(std::string filepath, int r, int g, int b) {
    // Eliminamos una textura previa si existe
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());
    if (loadedSurface == NULL) {
        throw SDLException("Error in texture load: IMG_Load()", SDL_GetError());
    }

    // Color key image (transparentar el fondo)
    if (r >= 0 && g >= 0 && b >= 0) {
        if (SDL_SetColorKey(loadedSurface, SDL_TRUE,
                            SDL_MapRGB(loadedSurface->format, r, g, b))) {
            throw SDLException("Error in texture load: SDL_SetColorKey()",
                               SDL_GetError());
        }
    }

    // Creamos la textura desde la surface
    newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (newTexture == NULL) {
        throw SDLException(
            "Error in texture load: SDL_CreateTextureFromSurface()",
            SDL_GetError());
    }

    // Dimensiones
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    mTexture = newTexture;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) const {
    if (SDL_SetTextureColorMod(mTexture, red, green, blue)) {
        fprintf(stderr, "Warning: Color modulation error!\n");
    }
}

void Texture::setBlendMode(SDL_BlendMode blending) const {
    if (SDL_SetTextureBlendMode(mTexture, blending)) {
        fprintf(stderr, "Warning: Blend mode error!\n");
    }
}

void Texture::setAlpha(Uint8 alpha) const {
    if (SDL_SetTextureAlphaMod(mTexture, alpha)) {
        fprintf(stderr, "Warning: Alpha modulation error!\n");
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle,
                     SDL_Point* center, SDL_RendererFlip flip) const {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    if (SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center,
                         flip)) {
        throw SDLException("Error in texture render: SDL_RenderCopy()",
                           SDL_GetError());
    }
}

int Texture::getWidth() const {
    return mWidth;
}

int Texture::getHeight() const {
    return mHeight;
}

void Texture::free() {
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

Texture::~Texture() {
    free();
}

//-----------------------------------------------------------------------------
