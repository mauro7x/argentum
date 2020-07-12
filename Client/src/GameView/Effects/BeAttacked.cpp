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
    if (!is_active) {
        current_frame = 0;
        is_active = true;
    }
}

void BeAttacked::act(const int it) {
    if (is_active) {
        current_frame += it;
        if (current_frame >= total_clips) {
            is_active = false;
            current_frame = 0;
        }
    }
}

void BeAttacked::render() const {
    if (is_active) {
        SDL_Rect render_quad = this->render_box;
        g_renderer->render(
            clips.at(current_frame / change_every_n_frames).getTexture(),
            &render_quad);
    }
}

BeAttacked::~BeAttacked() {}

//-----------------------------------------------------------------------------
