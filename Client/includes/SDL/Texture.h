#ifndef __TEXTURE_H__
#define __TEXTURE_H__

//-----------------------------------------------------------------------------
#include <stdio.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum TextType { SOLID_TEXT, BLENDED_TEXT };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Texture {
   private:
    SDL_Texture* texture = NULL;
    int w = 0, h = 0;

   public:
    /* Constructor */
    Texture();

    /* Deshabilitamos el constructor por copia. */
    Texture(const Texture&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Texture& operator=(const Texture&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Texture(Texture&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Texture& operator=(Texture&& other) = delete;

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
                              const std::string& text,
                              SDL_Color color = SDL_Color({0xFF, 0xFF, 0xFF,
                                                           0xFF}),
                              TextType type = BLENDED_TEXT);

    /* Carga la textura desde un texto y la wrappea a una anchura */
    void loadFromRenderedWrappedText(const Renderer* renderer, TTF_Font* font,
                                     const std::string& text, int max_width,
                                     SDL_Color color = SDL_Color({0xFF, 0xFF,
                                                                  0xFF, 0xFF}));

    /* Devuelve la textura */
    SDL_Texture* getTexture() const;

    /* Devuelve la anchura */
    int getWidth() const;

    /* Devuelve la altura */
    int getHeight() const;

    /* Libera la textura */
    void free();

    /* Destructor */
    ~Texture();
};

//-----------------------------------------------------------------------------

#endif  // __TEXTURE_H__
