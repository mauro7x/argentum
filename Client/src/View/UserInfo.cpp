#include "../../includes/View/UserInfo.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserInfo::UserInfo(const Renderer* renderer, const Player& player)
    : HUDComponent(renderer),
      player(player),
      nickname_font(NULL),
      lvl_font(NULL) {}

void UserInfo::init(const json& config) {
    if (initialized) {
        throw Exception("UserInfo already initialized.");
    }

    // Offsets de renderizado
    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    // Nickname
    nickname_fontsize = config["components"]["nickname"]["fontsize"];
    nickname_box.x = config["components"]["nickname"]["offset"]["x"];
    nickname_box.x += render_rect.x;
    nickname_box.y = config["components"]["nickname"]["offset"]["y"];
    nickname_box.y += render_rect.y;
    nickname_box.w = config["components"]["nickname"]["w"];
    nickname_box.h = config["components"]["nickname"]["h"];

    // Level
    lvl_fontsize = config["components"]["lvl"]["fontsize"];
    lvl_box.x = config["components"]["lvl"]["offset"]["x"];
    lvl_box.x += render_rect.x;
    lvl_box.y = config["components"]["lvl"]["offset"]["y"];
    lvl_box.y += render_rect.y;
    lvl_box.w = config["components"]["lvl"]["w"];
    lvl_box.h = config["components"]["lvl"]["h"];

    initialized = true;
}

void UserInfo::loadMedia() {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Cargamos las fuentes a usar
    nickname_font = TTF_OpenFont(FONT_OLDLONDON_FP, nickname_fontsize);
    lvl_font = TTF_OpenFont(FONT_OLDLONDON_FP, lvl_fontsize);

    if (!nickname_font /*|| !lvl_font*/) {
        throw Exception("UserInfo::init: Error opening TTF_Font/s.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INFO_BASE_FP);

    // proxy nickname
    current_nickname = "Mauro";
    nickname.loadFromRenderedText(g_renderer, nickname_font,
                                  current_nickname.c_str(),
                                  SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(nickname_pos, nickname, nickname_box);

    // proxy level
    current_lvl = 17;
    lvl.loadFromRenderedText(g_renderer, lvl_font, std::to_string(current_lvl),
                             SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(lvl_pos, lvl, lvl_box);
}

void UserInfo::update() {
    // fijarse si cambio el lvl (el nombre no cambia)
}

void UserInfo::render() const {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);

    // Renderizamos los componentes
    render_quad = {nickname_pos.x, nickname_pos.y, nickname.getWidth(),
                   nickname.getHeight()};
    g_renderer->render(nickname.getTexture(), &render_quad);

    render_quad = {lvl_pos.x, lvl_pos.y, lvl.getWidth(), lvl.getHeight()};
    g_renderer->render(lvl.getTexture(), &render_quad);
}

void UserInfo::free() {
    if (nickname_font) {
        TTF_CloseFont(nickname_font);
        nickname_font = NULL;
    }

    if (lvl_font) {
        TTF_CloseFont(lvl_font);
        lvl_font = NULL;
    }
}

UserInfo::~UserInfo() {}

//-----------------------------------------------------------------------------
