#include "../../../includes/GameView/Effects/EffectPlayer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EffectPlayer::EffectPlayer(const Renderer* g_renderer, const Camera& camera)
    : media_loaded(false),
      g_renderer(g_renderer),
      be_attacked_effect(g_renderer),
      be_healed_effect(g_renderer) {
    // Creamos los efectos

    // Death
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(DEATH_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Damage
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(DMG_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Explosion
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(EXPLOSION_SPELL_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Fire
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(FIRE_SPELL_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));

    // Lightning
    effects.emplace(std::piecewise_construct,
                    std::forward_as_tuple(LIGHTNING_SPELL_EV_TYPE),
                    std::forward_as_tuple(g_renderer, camera));
}

void EffectPlayer::loadMedia() {
    if (media_loaded) {
        throw Exception("EffectPlayer::loadMedia: media already loaded.");
    }

    json effects_config = JSON::loadJsonFile(paths::config(EFFECTS_FILEPATH));

    // Cargamos los efectos del juego
    {
        json& game_effects = effects_config["game_effects"];
        effects.at(DEATH_EV_TYPE).loadMedia(game_effects["death"]);
        effects.at(DMG_EV_TYPE).loadMedia(game_effects["dmg"]);
        effects.at(EXPLOSION_SPELL_EV_TYPE)
            .loadMedia(game_effects["explosion"]);
        effects.at(FIRE_SPELL_EV_TYPE).loadMedia(game_effects["fire"]);
        effects.at(LIGHTNING_SPELL_EV_TYPE)
            .loadMedia(game_effects["lightning"]);
    }

    // Cargamos los efectos de borde
    {
        json& border_effects = effects_config["border_effects"];
        be_attacked_effect.loadMedia(border_effects["be_attacked"]);
        be_healed_effect.loadMedia(border_effects["be_healed"]);
    }

    media_loaded = true;
}

void EffectPlayer::add(int event_id, const SDL_Point& pos) {
    if (!media_loaded) {
        throw Exception("EffectPlayer::add: media not loaded.");
    }

    switch (event_id) {
        case BEATTACKED_EV_TYPE: {
            be_attacked_effect.add();
            break;
        }

        case BEHEALED_EV_TYPE: {
            be_healed_effect.add();
            break;
        }

        default: {
            if (effects.count(event_id) > 0) {
                // No nos quejamos si no existe, para no hacer distinción en el
                // evento que dispare la reproducción.
                effects.at(event_id).add(pos);
            }
            break;
        }
    }
}

void EffectPlayer::act(const int it) {
    if (!media_loaded) {
        throw Exception("EffectPlayer::act: media not loaded.");
    }

    for (auto i = effects.begin(); i != effects.end(); i++) {
        i->second.act(it);
    }

    be_attacked_effect.act(it);
    be_healed_effect.act(it);
}

void EffectPlayer::render() const {
    if (!media_loaded) {
        throw Exception("EffectPlayer::render: media not loaded.");
    }

    for (auto it = effects.begin(); it != effects.end(); it++) {
        it->second.render();
    }

    be_attacked_effect.render();
    be_healed_effect.render();
}

EffectPlayer::~EffectPlayer() {}

//-----------------------------------------------------------------------------
