#include "../../includes/SDL/Mixer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Mixer::_increaseMusicVolume() {
    if (music_volume < MIX_MAX_VOLUME) {
        music_volume += VOLUME_LEVEL;
        Mix_VolumeMusic(music_volume);
    }
}

void Mixer::_decreaseMusicVolume() {
    if (music_volume > 0) {
        music_volume -= VOLUME_LEVEL;
        Mix_VolumeMusic(music_volume);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Mixer::Mixer() {}

void Mixer::init() {
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
    }

    // Cargamos la función que encadena canciones
    // Mix_HookMusicFinished();

    // Cargamos los chunks
    /*
    {
        json& chunks_to_load = audio_to_load["chunks"];
    }
    */
}

void Mixer::handleEvent(const SDL_Event& e) {
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
                fadeOutMusic();
                music.prev();
                playMusic(true);
                break;
            }

            case SDLK_RIGHT: {
                fadeOutMusic();
                music.next();
                playMusic(true);
                break;
            }

            default: {
                break;
            }
        }
    }
}

void Mixer::playMusic(bool fade_in) const {
    if (fade_in) {
        if (Mix_FadeInMusic(music.getCurrentValue(), -1, FADE_IN_MUSIC_MS)) {
            throw Exception(
                "Mixer::playHomeMusic: error in Mix_FadeInMusic. Mixer error: "
                "%s.",
                Mix_GetError());
        }
    } else {
        if (Mix_PlayMusic(music.getCurrentValue(), -1)) {
            throw Exception(
                "Mixer::playHomeMusic: error in Mix_PlayMusic. Mixer error: "
                "%s.",
                Mix_GetError());
        }
    }
}

void Mixer::fadeOutMusic() const {
    /* Documentación: devuelve 1 en caso de éxito, 0 en caso de error. */
    if (!Mix_FadeOutMusic(FADE_OUT_MUSIC_MS)) {
        throw Exception(
            "Mixer::fadeOutMusic: error while fading out music. Mixer error: "
            "%s.",
            Mix_GetError());
    }
}

void Mixer::stopMusic() const {
    /* Siempre retorna 0 por documentación, no podemos chequear errores */
    Mix_HaltMusic();
}

void Mixer::musicFinishedCallback() {
    fprintf(stderr, "proxy: la canción terminó.\n");
}

Mixer::~Mixer() {
    // Liberamos la música
    for (auto it = music.begin(); it != music.end(); it++) {
        Mix_FreeMusic(*it);
    }
}

//-----------------------------------------------------------------------------