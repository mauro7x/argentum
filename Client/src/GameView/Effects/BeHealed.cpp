#include "../../../includes/GameView/Effects/BeHealed.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

BeHealed::BeHealed(const Renderer* g_renderer) : g_renderer(g_renderer) {}

void BeHealed::loadMedia(const json& config) {
    json camera_config =
        JSON::loadJsonFile(paths::config(VIEWS_CONFIG_FILEPATH));

    // Config
    frames = config["frames"];
    clips_per_frame = config["clips_per_frame"];

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
}

void BeHealed::add() {
    if (!is_active) {
        is_active = true;
        is_new = true;
        current_frame = 0;
    }
}

void BeHealed::act(const int it) {
    if (is_active) {
        if (!is_new) {
            current_frame += it;
        } else {
            is_new = false;
        }

        if (current_frame >= frames) {
            is_active = false;
            current_frame = 0;
        }
    }
}

void BeHealed::render() const {
    if (is_active) {
        SDL_Rect render_quad = {0};
        for (size_t i = 0; i < clips_per_frame; i++) {
            size_t random_clip_idx = random((int)0, (int)(total_clips - 1));
            const Texture& random_clip = clips.at(random_clip_idx);
            render_quad = {0, 0, random_clip.getWidth(),
                           random_clip.getHeight()};
            render_quad.x =
                random((int)render_box.x - render_quad.w,
                       (int)(render_box.x + render_box.w - render_quad.w));
            render_quad.y =
                random((int)render_box.y - render_quad.h,
                       (int)(render_box.y + render_box.h - render_quad.h));
            g_renderer->render(random_clip.getTexture(), &render_quad);
        }
    }
}

BeHealed::~BeHealed() {}

//-----------------------------------------------------------------------------
