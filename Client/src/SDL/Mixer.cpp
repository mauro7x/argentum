#include "../../includes/SDL/Mixer.h"

//-----------------------------------------------------------------------------
// Callback para chain-music

static void musicCallback() {
    Mixer::finishedSongCallback();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Callback para conteo de chunks activos

static void chunkCallback(int channel) {
    Mixer::finishedChunkCallback();
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

void Mixer::setGameMusicVolume() {
    Mixer::getInstance()._setMusicVolume(GAME_MUSIC_VOLUME);
}

void Mixer::playEventSound(uint8_t sound_id, const SDL_Point& player_pos,
                           const SDL_Point& sound_pos) {
    Mixer::getInstance()._playChunk(sound_id, player_pos, sound_pos);
}

void Mixer::playLocalSound(LocalSound local_sound) {
    Mixer::getInstance()._playLocalSound(local_sound);
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

void Mixer::finishedChunkCallback() {
    Mixer::getInstance().active_chunks--;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos

void Mixer::_init() {
    json audio_to_load = JSON::loadJsonFile(paths::config(AUDIO_FILEPATH));

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
        dirpath = paths::asset(dirpath.c_str());
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

    {
        std::string dirpath, extension, filepath;
        dirpath = audio_to_load["chunks"]["dirpath"];
        dirpath = paths::asset(dirpath.c_str());
        extension = audio_to_load["chunks"]["extension"];
        listening_radio = (size_t)audio_to_load["chunks"]["listening_radio"];

        json& samples = audio_to_load["chunks"]["samples"];
        size_t n_samples = samples.size();
        chunks.reserve(n_samples);

        Mix_Chunk* chunk;
        for (size_t chunk_idx = 0; chunk_idx < n_samples; chunk_idx++) {
            filepath =
                dirpath + std::to_string((int)samples[chunk_idx]) + extension;
            chunk = Mix_LoadWAV(filepath.c_str());
            if (!chunk) {
                throw Exception(
                    "Mixer::init: error opening chunk [%s]. Mixer error: "
                    "%s.",
                    filepath.c_str(), Mix_GetError());
            }
            chunks.emplace(std::piecewise_construct,
                           std::forward_as_tuple(samples[chunk_idx]),
                           std::forward_as_tuple(chunk));
        }

        // Cargamos la función que lleva un conteo de chunks activos
        Mix_ChannelFinished(chunkCallback);
    }

    // Cargamos los sonidos locales
    {
        std::string filepath;
        Mix_Chunk* new_sound = NULL;

        // Click
        filepath = audio_to_load["local_sounds"]["click"];
        filepath = paths::asset(filepath.c_str());
        new_sound = Mix_LoadWAV(filepath.c_str());
        local_sounds[CLICK_SOUND] = new_sound;

        // Selección
        filepath = audio_to_load["local_sounds"]["selection"];
        filepath = paths::asset(filepath.c_str());
        new_sound = Mix_LoadWAV(filepath.c_str());
        local_sounds[SELECTION_SOUND] = new_sound;
    }
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

            case SDLK_F1: {
                if (muted) {
                    muted = false;
                    Mix_ResumeMusic();
                } else {
                    muted = true;
                    Mix_PauseMusic();
                }
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

void Mixer::_setMusicVolume(int volume) {
    if (volume > 0 && volume <= MIX_MAX_VOLUME) {
        music_volume = volume;
        Mix_VolumeMusic(music_volume);
    }
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

void Mixer::_playChunk(uint8_t sound_id, const SDL_Point& player_pos,
                       const SDL_Point& sound_pos) {
    if (chunks.count(sound_id) == 0) {
        // sonido sin efecto
        return;
    }

    if ((((sound_pos.x != player_pos.x) || (sound_pos.y != player_pos.y)) &&
         (active_chunks >= MAX_CHUNKS_SIMULTANEOUSLY)) ||
        muted) {
        // no reproducimos el sonido pues no es prioritario
        return;
    }

    size_t distance = std::abs(player_pos.x - sound_pos.x) +
                      std::abs(player_pos.y - sound_pos.y);
    if (distance > listening_radio) {
        // no reproducimos el sonido pues está muy lejos
        return;
    }

    // Reproducimos el sonido
    Mix_Chunk* sound = NULL;
    try {
        sound = chunks.at(sound_id);
    } catch (...) {
        throw Exception("Mixer::_playChunk: invalid sound_id (%u)", sound_id);
    }

    int channel = Mix_PlayChannel(-1, sound, 0);
    if (channel < 0) {
        // No hay más canales disponibles
        return;
    }

    active_chunks++;

    // Agregamos efectos
    Sint16 sdl_angle = _getAngle(player_pos, sound_pos);
    Uint8 sdl_distance = (distance * MAX_DISTANCE_EFFECT) / listening_radio;

    Mix_SetPosition(channel, sdl_angle, sdl_distance);
}

void Mixer::_playLocalSound(LocalSound local_sound) {
    if ((local_sound < 0) || (local_sound > N_LOCAL_SOUNDS)) {
        throw Exception("Mixer::_playLocalSound: invalid local_sound (%u)",
                        (unsigned int)local_sound);
    }

    if (muted) {
        return;
    }

    int channel = Mix_PlayChannel(-1, local_sounds[local_sound], 0);
    if (channel < 0) {
        // No hay más canales disponibles
        return;
    }

    active_chunks++;

    // Limpiamos los efectos
    Mix_SetPosition(channel, 0, 0);
}

Sint16 Mixer::_getAngle(const SDL_Point& origin, const SDL_Point& point) const {
    SDL_Point centered = {point.x - origin.x, point.y - origin.y};

    if (centered.x >= 0 && centered.y > 0) {
        // 1C
        return RADTODEG(std::atan(centered.x / centered.y));
    }

    if (centered.x > 0 && centered.y == 0) {
        return 90;
    }

    if (centered.x > 0 && centered.y < 0) {
        // 2C
        return 180 - RADTODEG(std::atan(centered.x / (-centered.y)));
    }

    if (centered.x < 0 && centered.y < 0) {
        // 3C
        return RADTODEG(std::atan(centered.x / centered.y)) + 180;
    }

    if (centered.x < 0 && centered.y == 0) {
        return 270;
    }

    if (centered.x < 0 && centered.y > 0) {
        // 4C
        return 360 - RADTODEG(std::atan((-centered.x) / centered.y));
    }

    return 0;
}

Mixer::~Mixer() {
    // Liberamos la música
    for (auto it = music.begin(); it != music.end(); it++) {
        Mix_FreeMusic(*it);
    }

    // Liberamos chunks
    for (auto it = chunks.begin(); it != chunks.end(); it++) {
        Mix_FreeChunk(it->second);
    }

    // Liberamos local sounds
    for (int i = 0; i < N_LOCAL_SOUNDS; i++) {
        if (local_sounds[i] != NULL) {
            Mix_FreeChunk(local_sounds[i]);
            local_sounds[i] = NULL;
        }
    }
}

//-----------------------------------------------------------------------------