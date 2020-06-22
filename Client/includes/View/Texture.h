#ifndef __TEXTURE_H__
#define __TEXTURE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include <string>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum TextType { SOLID_TEXT, BLENDED_TEXT };
//-----------------------------------------------------------------------------

class Texture {
   private:
    SDL_Texture* texture;

    // Dimensiones
    int width;
    int height;

   public:
    /* Constructor */
    Texture();

    /* Deshabilitamos el constructor por copia. */
    Texture(const Texture&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Texture& operator=(const Texture&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Texture(Texture&& other);

    /* Habilitamos el operador= para movimiento. */
    Texture& operator=(Texture&& other);

    /**
     * Descripcion: carga la textura desde un archivo.
     * En caso de proporcionarse un rectangulo de scale, se escala la imagen.
     * En caso de proporcionarse valores de RGB, se transparenta la imagen con
     * dicho color.
     */
    void loadFromFile(const Renderer* renderer, std::string filepath,
                      int r = -1, int g = -1, int b = -1);

    /* Carga la textura desde un texto */
    void loadFromRenderedText(const Renderer* renderer, TTF_Font* font,
                              std::string text, SDL_Color color,
                              TextType type = BLENDED_TEXT);

    /* Settea la modulación de los colores */
    void setColor(Uint8 red, Uint8 green, Uint8 blue) const;

    /* Settea el blending mode */
    void setBlendMode(SDL_BlendMode blending) const;

    /* Settea la transparencia */
    void setAlpha(Uint8 alpha) const;

    /* Devuelve la textura */
    SDL_Texture* getTexture() const;

    /* Devuelve la anchura */
    int getWidth() const;

    /* Devuelve la altura */
    int getHeight() const;

    /* Libera recursos */
    void free();

    /* Destructor */
    ~Texture();
};

//-----------------------------------------------------------------------------

#endif  // __TEXTURE_H__
