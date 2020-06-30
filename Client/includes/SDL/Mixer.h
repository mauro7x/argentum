#ifndef __MIXER_H__
#define __MIXER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Mixer {
   private:
    //-----------------------------------------------------------------------------
    // Atributos
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Mixer();

    /* Deshabilitamos el constructor por copia. */
    Mixer(const Mixer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Mixer& operator=(const Mixer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Mixer(Mixer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Mixer& operator=(Mixer&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública
    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Mixer();
};

//-----------------------------------------------------------------------------

#endif  // __MIXER_H__