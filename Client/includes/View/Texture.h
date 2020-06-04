#ifndef __TEXTURE_H__
#define __TEXTURE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <string>

#include "../../../Common/includes/Exceptions/SDLException.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Wrapper para textura SDL. */
class Texture {
   private:
    SDL_Texture* mTexture;
    SDL_Renderer* mRenderer;

    // Dimensiones
    int mWidth;
    int mHeight;

   public:
    /* Constructor */
    Texture(SDL_Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    Texture(const Texture&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Texture& operator=(const Texture&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Texture(Texture&& other);

    /* habilitamos el operador= para movimiento. */
    Texture& operator=(Texture&& other);

    /**
     * Descripcion: carga la textura desde un archivo.
     * En caso de proporcionarse un rectangulo de scale, se escala la imagen.
     * En caso de proporcionarse valores de RGB, se transparenta la imagen con
     * dicho color.
     */
    void loadFromFile(std::string filepath, int r = -1, int g = -1, int b = -1);

    /* Settea la modulación de los colores */
    void setColor(Uint8 red, Uint8 green, Uint8 blue) const;

    /* Settea el blending mode */
    void setBlendMode(SDL_BlendMode blending) const;

    /* Settea la transparencia */
    void setAlpha(Uint8 alpha) const;

    /* Renderiza la textura. Se le puede pasar el área a renderizar. */
    void render(int x, int y, SDL_Rect* clip = NULL, SDL_Rect* scale = NULL,
                double angle = 0.0, SDL_Point* center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE) const;

    /* Libera recursos */
    void free();

    /* Destructor */
    ~Texture();
};

//-----------------------------------------------------------------------------

#endif  // __TEXTURE_H__
