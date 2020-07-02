#ifndef __MIXER_H__
#define __MIXER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
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
#define MAX_DISTANCE_EFFECT 255
#define RADTODEG(rad) rad*(180.0 / M_PI)

// Volumen (el máximo volumen es 128)
#define MUSIC_VOLUME_LEVEL 4

// Chunks
#define MAX_CHUNKS_SIMULTANEOUSLY 3 /* límite para sonidos EXTERNOS */
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

    /* Método para hacer sonar un chunk */
    static void playEventSound(uint8_t sound_id, const SDL_Point& player_pos,
                               const SDL_Point& sound_pos);

    /* Callback que se llama cuando termina una canción */
    static void finishedSongCallback();

    /* Callback que se llama cuando termina un chunk */
    static void finishedChunkCallback();

    //-------------------------------------------------------------------------

   private:
    //-------------------------------------------------------------------------
    // Atributos

    // Música
    int music_volume = 0;
    RoundRobinList<Mix_Music*> music;

    // Chunks
    size_t listening_radio = 0;
    int active_chunks = 0;
    std::vector<Mix_Chunk*> chunks;

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
    // Chunks

    /* Reproduce un chunk si está dentro del listening_radio */
    void _playChunk(uint8_t sound_id, const SDL_Point& player_pos,
                    const SDL_Point& sound_pos);

    /* Obtiene el angulo entre dos posiciones según la convención de
     * SDL_SetPosition */
    Sint16 _getAngle(const SDL_Point& origin, const SDL_Point& point) const;

    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------

#endif  // __MIXER_H__