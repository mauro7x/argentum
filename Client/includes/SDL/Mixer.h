#ifndef __MIXER_H__
#define __MIXER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../RoundRobinList.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Efectos
#define FADE_IN_MUSIC_MS 2000
#define FADE_OUT_MUSIC_MS 2000

// Volumen (el máximo volumen es 128)
#define VOLUME_LEVEL 8
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Mixer {
   private:
    // Música
    int music_volume = 0;
    RoundRobinList<Mix_Music*> music;

    // Chunks

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Sube el volumen actual de la música */
    void _increaseMusicVolume();

    /* Baja el volumen actual de la música */
    void _decreaseMusicVolume();

    //-------------------------------------------------------------------------

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

    //-------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga el audio necesario */
    void init();

    /* Handlea un evento de SDL */
    void handleEvent(const SDL_Event& e);

    //-------------------------------------------------------------------------
    // Música

    /* Reproduce música. El flag permite reproducirla con fadein. */
    void playMusic(bool fade_in = false) const;

    /* Realiza un desvanecimiento de la música actual */
    void fadeOutMusic() const;

    /* Detiene cualquier música que esté sonando */
    void stopMusic() const;

    /* Callback para cuando una canción termina */
    void musicFinishedCallback();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Mixer();
};

//-----------------------------------------------------------------------------

#endif  // __MIXER_H__