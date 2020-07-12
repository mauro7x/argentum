#include "../../../includes/GameView/Effects/BeAttacked.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

BeAttacked::BeAttacked(const Renderer* g_renderer) : g_renderer(g_renderer) {}

void BeAttacked::loadMedia(const json& config) {
    json camera_config =
        JSON::loadJsonFile(paths::config(VIEWS_CONFIG_FILEPATH));

    // Render quad
    render_box.x = camera_config["camera"]["offset"]["x"];
    render_box.y = camera_config["camera"]["offset"]["y"];
    render_box.w = camera_config["camera"]["w"];
    render_box.h = camera_config["camera"]["h"];

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

        clips.at(i).loadFromFile(g_renderer, filepath);
    }

    // Change every n frames
    change_every_n_frames = config["change_every_n_frames"];
    total_clips *= change_every_n_frames;
}

void BeAttacked::add() {
    active_effects.emplace_back();
}

void BeAttacked::act(const int it) {
    for (auto i = active_effects.begin(); i != active_effects.end();) {
        // Avanzamos el clip si no fue creado en la misma iteración
        if (!(i->is_new)) {
            i->current_clip += it;
        } else {
            i->is_new = false;
        }

        fprintf(stderr, "efecto en su iteración %i\n", (int)i->current_clip);

        // Si la animación terminó, la eliminamos
        if (i->current_clip < total_clips) {
            i++;
        } else {
            fprintf(stderr, "efecto siendo eliminado\n");
            i = active_effects.erase(i);
        }
    }
}

void BeAttacked::render() const {
    SDL_Rect render_quad;
    for (auto it = active_effects.begin(); it != active_effects.end(); it++) {
        render_quad = this->render_box;
        g_renderer->render(
            clips.at(it->current_clip / change_every_n_frames).getTexture(),
            &render_quad);
    }
}

BeAttacked::~BeAttacked() {}

//-----------------------------------------------------------------------------
