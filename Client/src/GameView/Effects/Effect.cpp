#include "../../../includes/GameView/Effects/Effect.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Effect::_setRenderQuad(SDL_Rect& render_quad, const int x_tile,
                            const int y_tile) const {
    // Agregamos el offset del tile
    render_quad.x = x_tile * TILE_WIDTH;
    render_quad.y = y_tile * TILE_HEIGHT;

    // Centramos el efecto
    render_quad.x += (TILE_WIDTH - render_quad.w) / 2;
    render_quad.y += (TILE_HEIGHT - render_quad.h) / 2;

    // Offset
    render_quad.x += x_offset;
    render_quad.y += y_offset;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Effect::Effect(const Renderer* g_renderer, const Camera& camera)
    : g_renderer(g_renderer), camera(camera) {}

void Effect::loadMedia(const json& config) {
    // Configuración del clip
    clip_needed = config["clip"]["is_needed"];
    if (clip_needed) {
        clip_rect.x = 0;
        clip_rect.y = 0;
        clip_rect.w = config["clip"]["w"];
        clip_rect.h = config["clip"]["h"];
        clip = &clip_rect;
    } else {
        clip = NULL;
    }

    // Configuración del transparentado
    int r = -1, g = -1, b = -1;
    if (config["color_key"]["is_needed"]) {
        r = config["color_key"]["color"]["r"];
        g = config["color_key"]["color"]["g"];
        b = config["color_key"]["color"]["b"];
    }

    // Offset
    x_offset = config["offset"]["x"];
    y_offset = config["offset"]["y"];

    // Cargado de los clips
    total_clips = (size_t)config["clips"];
    clips.reserve(total_clips);
    std::string dirpath, filepath;
    dirpath = config["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    for (size_t i = 0; i < total_clips; i++) {
        filepath = dirpath + std::to_string(i) + ".png";
        clips.emplace(std::piecewise_construct, std::forward_as_tuple(i),
                      std::forward_as_tuple());

        clips.at(i).loadFromFile(g_renderer, filepath, r, g, b);
    }

    // Change every n frames
    change_every_n_frames = config["change_every_n_frames"];
    total_clips *= change_every_n_frames;
}

void Effect::add(const SDL_Point& pos) {
    active_effects.emplace_back(pos);
}

void Effect::act(const int it) {
    for (auto i = active_effects.begin(); i != active_effects.end();) {
        // Avanzamos el clip si no fue creado en la misma iteración
        if (!(i->is_new)) {
            i->current_clip += it;
        } else {
            i->is_new = false;
        }

        // Si la animación terminó, la eliminamos
        if (i->current_clip < total_clips) {
            i++;
        } else {
            i = active_effects.erase(i);
        }
    }
}

void Effect::render() const {
    SDL_Rect render_quad = {0};

    for (auto it = active_effects.begin(); it != active_effects.end(); it++) {
        const Texture& texture =
            clips.at(it->current_clip / change_every_n_frames);
        if (clip) {
            render_quad.w = clip->w;
            render_quad.h = clip->h;
        } else {
            render_quad.w = texture.getWidth();
            render_quad.h = texture.getHeight();
        }

        _setRenderQuad(render_quad, it->effect_pos.x, it->effect_pos.y);
        camera.renderIfVisible(g_renderer, texture.getTexture(), render_quad,
                               clip);
    }
}

Effect::~Effect() {}

//-----------------------------------------------------------------------------
