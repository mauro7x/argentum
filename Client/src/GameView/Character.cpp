#include "../../includes/GameView/Character.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Character::_copyData(const CharacterData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Data del character
    nickname = init_data.nickname;
    head_id = init_data.head_id;
    body_id = init_data.body_id;
    equipment = init_data.equipment;
    level = init_data.level;
}

void Character::_renderInfo() const {
    // Primero el nivel
    SDL_Rect level_quad = {0, 0, info_level.getWidth(), info_level.getHeight()};

    // Centramos las coordenadas
    level_quad.x = (TILE_WIDTH - level_quad.w) / 2;
    int unit_height;
    if (head_id) {
        unit_height = g_sprites->get(head_id).clip_h;
    } else {
        unit_height = g_sprites->get(body_id).clip_h;
    }
    level_quad.y = (TILE_HEIGHT * (0.8)) - level_quad.h - unit_height -
                   INFO_SPACE_FROM_HEAD;

    // Le agregamos offsets de la unidad
    level_quad.x += (int)this->x;
    level_quad.y += (int)this->y;

    SDL_Rect nick_quad = level_quad;
    g_camera.renderIfVisible(g_renderer, info_level.getTexture(), level_quad);

    // Ahora el nickname
    nick_quad.w = info_nickname.getWidth();
    nick_quad.h = info_nickname.getHeight();
    nick_quad.x = (TILE_WIDTH - nick_quad.w) / 2;
    nick_quad.x += (int)this->x;
    nick_quad.y -= nick_quad.h;

    // Sombra
    SDL_Rect nick_quad_bg = nick_quad;
    nick_quad_bg.y++;

    g_camera.renderIfVisible(g_renderer, info_nickname_shadow.getTexture(),
                             nick_quad_bg);
    g_camera.renderIfVisible(g_renderer, info_nickname.getTexture(), nick_quad);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Character::Character(Renderer* renderer, const Camera& camera,
                     UnitSpriteContainer* sprites, TTF_Font* g_nickname_font,
                     TTF_Font* g_level_font)
    : Unit(renderer, camera, sprites),
      g_nickname_font(g_nickname_font),
      g_level_font(g_level_font) {}

void Character::init(const CharacterData& init_data) {
    if (state) {
        throw Exception("Character has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Cargamos la info */
    if (!g_nickname_font || !g_level_font) {
        throw Exception("Player::init: Fonts not initialized.");
    }

    info_nickname.loadFromRenderedText(g_renderer, g_nickname_font, nickname,
                                       CHARACTER_NICKNAME_COLOR);
    info_nickname_shadow.loadFromRenderedText(
        g_renderer, g_nickname_font, nickname, SDL_Color({0, 0, 0, 255}));
    info_level.loadFromRenderedText(g_renderer, g_level_font,
                                    "Nivel " + std::to_string(level));

    /* Seteamos nuestra posición en pixeles para el renderizado */
    x = TILE_WIDTH * data.x_tile;
    y = TILE_HEIGHT * data.y_tile;

    /* Completamos la inicialización */
    state = READY;
}

void Character::update(const CharacterData& updated_data) {
    if (!state) {
        throw Exception(
            "Character has not been initialized (update requested).");
    }

    /* Verificamos si hay que modificar la info */
    if (level != updated_data.level) {
        info_level.loadFromRenderedText(g_renderer, g_level_font,
                                        "Nivel " + std::to_string(level));
    }

    /* Actualizamos la data */
    _copyData(updated_data);

    /* Iniciamos el movimiento si nuestra posición en pixeles no coincide*/
    _setMovementSpeed();
}

void Character::render() const {
    if (!state) {
        throw Exception(
            "Character has not been initialized (render requested).");
    }

    // Cuerpo
    if (body_id) {
        _render(g_sprites->get(body_id));
    }

    // Armadura
    if (equipment[ARMOUR]) {
        _render(g_sprites->get(equipment[ARMOUR]));
    }

    // Escudo
    if (equipment[SHIELD]) {
        _render(g_sprites->get(equipment[SHIELD]));
    }

    // Espada
    if (equipment[WEAPON]) {
        _render(g_sprites->get(equipment[WEAPON]));
    }

    // Cabeza
    if (head_id) {
        _render(g_sprites->get(head_id));
    }

    // Casco
    if (equipment[HELMET]) {
        _render(g_sprites->get(equipment[HELMET]));
    }

    // Info
    _renderInfo();
}

Character::~Character() {}

//-----------------------------------------------------------------------------
