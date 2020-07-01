#include "../../includes/SDL/Mixer.h"

//-----------------------------------------------------------------------------
// Callback para chain-music

static void musicCallback() {
    Mixer::finishedSongCallback();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Singleton

Mixer& Mixer::getInstance() {
    static Mixer instance;
    return instance;
}

Mixer::Mixer() {}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API pública

void Mixer::init() {
    Mixer::getInstance()._init();
}

void Mixer::handleEvent(const SDL_Event& e) {
    Mixer::getInstance()._handleEvent(e);
}

void Mixer::playMusic(bool fade_in) {
    Mixer::getInstance()._playMusic(fade_in);
}

void Mixer::finishedSongCallback() {
    Mixer& instance = Mixer::getInstance();

    // Avanzamos la canción
    instance.music.next();

    // Rebobinamos la canción anterior para la próxima
    Mix_RewindMusic();

    // La ponemos a sonar
    instance._playMusic(true);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos

void Mixer::_init() {
    json audio_to_load = JSON::loadJsonFile(AUDIO_FILEPATH);

    // Seteamos el volumen inicial de la música (a la mitad del máximo)
    music_volume = (MIX_MAX_VOLUME / 2);
    if ((Mix_VolumeMusic(music_volume) == -1)) {
        fprintf(stderr,
                "WARNING | Mixer::init: error while setting the volume. Mixer "
                "error: %s.",
                Mix_GetError());
    }

    // Cargamos la música
    {
        std::string dirpath, extension, filepath;
        dirpath = audio_to_load["music"]["dirpath"];
        extension = audio_to_load["music"]["extension"];
        size_t songs = audio_to_load["music"]["songs"];
        Mix_Music* song;
        for (size_t song_idx = 0; song_idx < songs; song_idx++) {
            filepath = dirpath + std::to_string(song_idx) + extension;
            song = Mix_LoadMUS(filepath.c_str());
            if (!song) {
                throw Exception(
                    "Mixer::init: error opening music [%s]. Mixer error: "
                    "%s.",
                    filepath.c_str(), Mix_GetError());
            }
            music.pushBack(song);
        }

        // Cargamos la función que encadena canciones
        Mix_HookMusicFinished(musicCallback);
    }

    // Cargamos los chunks
    /*
    {
        json& chunks_to_load = audio_to_load["chunks"];
    }
    */
}

void Mixer::_handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP: {
                _increaseMusicVolume();
                break;
            }

            case SDLK_DOWN: {
                _decreaseMusicVolume();
                break;
            }

            case SDLK_LEFT: {
                // Cuando termine la canción, se avanzará a la siguiente, por lo
                // que para que suene la anterior, rebobinamos 2 veces.
                music.prev();
                music.prev();

                // Fadeout de la canción actual
                _stopMusic(true);
                break;
            }

            case SDLK_RIGHT: {
                // No es necesario avanzar pues cuando la canción termina,
                // avanza a la siguiente automáticamente (callback).

                // Fadeout de la canción actual
                _stopMusic(true);
                break;
            }

            default: {
                break;
            }
        }
    }
}

void Mixer::_playMusic(bool fade_in) const {
    if (fade_in) {
        if (Mix_FadeInMusic(music.getCurrentValue(), 0, FADE_IN_MUSIC_MS)) {
            throw Exception(
                "Mixer::_playMusic: error in Mix_FadeInMusic. Mixer error: "
                "%s.",
                Mix_GetError());
        }
    } else {
        if (Mix_PlayMusic(music.getCurrentValue(), 0)) {
            throw Exception(
                "Mixer::_playMusic: error in Mix_PlayMusic. Mixer error: "
                "%s.",
                Mix_GetError());
        }
    }

    Mix_RewindMusic();
}

void Mixer::_stopMusic(bool fade_out) const {
    if (fade_out) {
        if (!Mix_FadeOutMusic(FADE_OUT_MUSIC_MS)) {
            throw Exception(
                "Mixer::_stopMusic: error while fading out music. Mixer "
                "error: "
                "%s.",
                Mix_GetError());
        }
    } else {
        /* Siempre retorna 0 por documentación, no podemos chequear errores */
        Mix_HaltMusic();
    }
}

void Mixer::_musicFinishedCallback() {
    fprintf(stderr, "proxy: la canción terminó.\n");
}

void Mixer::_increaseMusicVolume() {
    if (music_volume < MIX_MAX_VOLUME) {
        music_volume += MUSIC_VOLUME_LEVEL;
        Mix_VolumeMusic(music_volume);
    }
}

void Mixer::_decreaseMusicVolume() {
    if (music_volume > 0) {
        music_volume -= MUSIC_VOLUME_LEVEL;
        Mix_VolumeMusic(music_volume);
    }
}

Mixer::~Mixer() {
    // Liberamos la música
    for (auto it = music.begin(); it != music.end(); it++) {
        Mix_FreeMusic(*it);
    }
}

//-----------------------------------------------------------------------------