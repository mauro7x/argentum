#include "../../../includes/GameView/Effects/EffectPlayer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EffectPlayer::EffectPlayer(const Renderer* g_renderer, const Camera& camera)
    : media_loaded(false), g_renderer(g_renderer) {
    // Creamos los efectos

    // Be Attacked
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(DEATH_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Explosion
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(EXPLOSION_SPELL_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Be Attacked
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(BEATTACKED_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));
}

void EffectPlayer::loadMedia() {
    if (media_loaded) {
        throw Exception("EffectPlayer::loadMedia: media already loaded.");
    }

    // Cargamos los efectos
    json effects_config = JSON::loadJsonFile(paths::config(EFFECTS_FILEPATH));
    effects.at(DEATH_EV_TYPE).loadMedia(effects_config["death"]);
    effects.at(EXPLOSION_SPELL_EV_TYPE).loadMedia(effects_config["explosion"]);
    effects.at(BEATTACKED_EV_TYPE).loadMedia(effects_config["dmg_received"]);

    media_loaded = true;
}

void EffectPlayer::add(int event_id, const SDL_Point& pos) {
    if (!media_loaded) {
        throw Exception("EffectPlayer::add: media not loaded.");
    }

    fprintf(stderr, "Llega evento %i al effectplayer...\n", event_id);

    if (effects.count(event_id) > 0) {
        // No nos quejamos si no existe, para no hacer distinción en el evento
        // que dispare la reproducción.

        fprintf(stderr, "Lo agregamos a los efectos a renderizar!\n");
        effects.at(event_id).add(pos);
    }
}

void EffectPlayer::act(const int it) {
    if (!media_loaded) {
        throw Exception("EffectPlayer::act: media not loaded.");
    }

    for (auto i = effects.begin(); i != effects.end(); i++) {
        i->second.act(it);
    }
}

void EffectPlayer::render() const {
    if (!media_loaded) {
        throw Exception("EffectPlayer::render: media not loaded.");
    }

    for (auto it = effects.begin(); it != effects.end(); it++) {
        it->second.render();
    }
}

EffectPlayer::~EffectPlayer() {}

//-----------------------------------------------------------------------------
