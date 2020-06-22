#include "../../includes/View/HUD.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HUD::HUD(const Renderer* renderer, const Player& player)
    : initialized(false),
      g_renderer(renderer),
      player(player),

      // Componentes
      console(renderer),
      user_info(renderer, player),
      user_inventory(renderer, player),
      user_stats(renderer, player) {}

void HUD::init(const json& config) {
    if (initialized) {
        throw Exception("HUD already initialized.");
    }

    // Iniciamos nuestros componentes
    console.init(config["console"]);
    user_info.init(config["user_info"]);
    user_inventory.init(config["user_inventory"]);
    user_stats.init(config["user_stats"]);

    // Iniciamos nuestros offsets de renderizado
    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void HUD::loadMedia() {
    if (!initialized) {
        throw Exception("HUD not initialized.");
    }

    // Cargamos la media de nuestros componentes
    console.loadMedia();
    user_info.loadMedia();
    user_inventory.loadMedia();
    user_stats.loadMedia();

    // Cargamos la upper layer
    upper_layer.loadFromFile(g_renderer, HUD_UPPER_LAYER_FP);
}

void HUD::update() {
    // updatear la info
}

void HUD::render() const {
    if (!initialized) {
        throw Exception("HUD not initialized.");
    }

    // Renderizamos nuestros componentes
    console.render();
    user_info.render();
    user_inventory.render();
    user_stats.render();

    // Renderizamos la upper layer
    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(upper_layer.getTexture(), &render_quad);
}

void HUD::free() {
    console.free();
    user_info.free();
    user_inventory.free();
    user_stats.free();
}

HUD::~HUD() {
    free();
}

//-----------------------------------------------------------------------------
