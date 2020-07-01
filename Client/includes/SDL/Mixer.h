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
// Singleton Mixer

class Mixer {
   public:
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

    /* Método para cargar la media necesaria */
    static void init();

    /* Método para handlear un evento de SDL */
    static void handleEvent(const SDL_Event& e);

    /* Método para que empiece a sonar la lista de canciones */
    static void playMusic(bool fade_in);

    /* Callback que se llama cuando termina una canción */
    static void finishedSongCallback();

    //-------------------------------------------------------------------------

   private:
    //-------------------------------------------------------------------------
    // Atributos

    // Música
    int music_volume = 0;
    RoundRobinList<Mix_Music*> music;

    // Chunks

    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Singleton

    /* Única instancia */
    static Mixer& getInstance();

    /* Constructor */
    Mixer();

    /* Destructor */
    ~Mixer();

    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Métodos

    /* Carga el audio necesario */
    void _init();

    /* Handlea un evento de SDL */
    void _handleEvent(const SDL_Event& e);

    //-------------------------------------------------------------------------
    // Música

    /* Reproduce música. El flag permite reproducirla con fadein. */
    void _playMusic(bool fade_in = false) const;

    /* Detiene cualquier música que esté sonando. El flag permite fade-out */
    void _stopMusic(bool fade_out = false) const;

    /* Callback para cuando una canción termina */
    void _musicFinishedCallback();

    /* Sube el volumen actual de la música */
    void _increaseMusicVolume();

    /* Baja el volumen actual de la música */
    void _decreaseMusicVolume();

    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------

#endif  // __MIXER_H__