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
      lvl_font(NULL),
      xp(renderer) {}

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

    // XP
    SDL_Point xp_offset = {0};
    xp_offset.x = config["components"]["xp_bar"]["offset"]["x"];
    xp_offset.x += render_rect.x;
    xp_offset.y = config["components"]["xp_bar"]["offset"]["y"];
    xp_offset.y += render_rect.y;
    Uint8 r = config["components"]["xp_bar"]["color"]["r"];
    Uint8 g = config["components"]["xp_bar"]["color"]["g"];
    Uint8 b = config["components"]["xp_bar"]["color"]["b"];
    Uint8 a = config["components"]["xp_bar"]["color"]["a"];
    xp.init(xp_offset, 0, 100, SDL_Color({r, g, b, a}), BOTH_PBTEXTYPE);

    initialized = true;
}

void UserInfo::loadMedia() {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Cargamos las fuentes a usar
    nickname_font = TTF_OpenFont(FONT_OLDLONDON_FP, nickname_fontsize);
    lvl_font = TTF_OpenFont(FONT_AUGUSTA_FP, lvl_fontsize);

    if (!nickname_font || !lvl_font) {
        throw Exception("UserInfo::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INFO_BASE_FP);

    // Nickname loading
    current_nickname = "loading";
    nickname.loadFromRenderedText(g_renderer, nickname_font,
                                  current_nickname.c_str(),
                                  SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(nickname_pos, nickname, nickname_box);

    // Level loading
    current_lvl = 0;
    lvl.loadFromRenderedText(g_renderer, lvl_font, std::to_string(current_lvl),
                             SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(lvl_pos, lvl, lvl_box);

    xp.loadMedia();
}

void UserInfo::update() {
    // Sólo nos interesa actualizar el username una vez
    if (!nickname_loaded) {
        current_nickname = player.getNickname();
        nickname.loadFromRenderedText(g_renderer, nickname_font,
                                      current_nickname.c_str(),
                                      SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        _center(nickname_pos, nickname, nickname_box);
        nickname_loaded = true;
    }

    // Update del nivel si es necesario
    uint32_t new_lvl = player.getLevel();
    if (current_lvl != new_lvl) {
        current_lvl = new_lvl;
        lvl.loadFromRenderedText(g_renderer, lvl_font,
                                 std::to_string(current_lvl),
                                 SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        _center(lvl_pos, lvl, lvl_box);
    }

    // Update de la experiencia
    xp.update(player.getExp(), player.getLvlUpExp());
}

void UserInfo::render() const {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);

    // Nickname
    render_quad = {nickname_pos.x, nickname_pos.y, nickname.getWidth(),
                   nickname.getHeight()};
    g_renderer->render(nickname.getTexture(), &render_quad);

    // Lvl
    render_quad = {lvl_pos.x, lvl_pos.y, lvl.getWidth(), lvl.getHeight()};
    g_renderer->render(lvl.getTexture(), &render_quad);

    // Barra de xp
    xp.render();
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

    xp.free();
}

UserInfo::~UserInfo() {
    free();
}

//-----------------------------------------------------------------------------
