#include "../../includes/GameView/Character.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Character::_copyData(const CharacterData& data) {
    // Data básica
    this->data = data.basic_data;

    // Data del character
    this->nickname = data.nickname;
    this->head_id = data.head_id;
    this->body_id = data.body_id;
    this->equipment = data.equipment;
    this->level = data.level;

    // Si es enano/gnomo
    this->is_shorter = data.is_shorter;
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

    // Sombra del nickname
    SDL_Rect nick_quad_bg = nick_quad;
    nick_quad_bg.y++;

    // Mensaje flotante
    SDL_Rect msg_quad = nick_quad;

    g_camera.renderIfVisible(g_renderer, info_nickname_shadow.getTexture(),
                             nick_quad_bg);
    g_camera.renderIfVisible(g_renderer, info_nickname.getTexture(), nick_quad);

    // Ahora el mensaje flotante (si existe)
    if (msg_active) {
        msg_quad.w = msg.getWidth();
        msg_quad.h = msg.getHeight();
        msg_quad.x = (TILE_WIDTH - msg_quad.w) / 2;
        msg_quad.x += (int)this->x;
        msg_quad.y -= (msg_quad.h + MSG_SPACE_FROM_NICK);

        // Sombra del msg
        SDL_Rect msg_quad_bg = msg_quad;
        msg_quad_bg.y++;

        g_camera.renderIfVisible(g_renderer, msg_shadow.getTexture(),
                                 msg_quad_bg);
        g_camera.renderIfVisible(g_renderer, msg.getTexture(), msg_quad);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Character::Character(const Renderer* renderer, const Camera& camera,
                     UnitSpriteContainer* sprites, TTF_Font* g_nickname_font,
                     TTF_Font* g_level_font, TTF_Font* g_msg_font)
    : Unit(renderer, camera, sprites),
      g_nickname_font(g_nickname_font),
      g_level_font(g_level_font),
      g_msg_font(g_msg_font) {}

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
        info_level.loadFromRenderedText(
            g_renderer, g_level_font,
            "Nivel " + std::to_string(updated_data.level));
    }

    /* Si hay que iniciar un movimiento */
    if ((data.x_tile != updated_data.basic_data.x_tile) ||
        (data.y_tile != updated_data.basic_data.y_tile)) {
        _setMovementSpeed(updated_data.basic_data.x_tile,
                          updated_data.basic_data.y_tile);
    }

    /* Actualizamos la data */
    _copyData(updated_data);
}

void Character::addMessage(const std::string& msg) {
    msg_active = true;
    msg_its = MSG_ITERATIONS;
    this->msg.loadFromRenderedWrappedText(g_renderer, g_msg_font, msg,
                                          MSG_MAX_WIDTH);
    this->msg_shadow.loadFromRenderedWrappedText(
        g_renderer, g_msg_font, msg, MSG_MAX_WIDTH, SDL_Color({0, 0, 0, 255}));
}

void Character::act(const int it) {
    Unit::act(it);

    if (msg_active) {
        msg_its -= it;
        if (msg_its <= 0) {
            msg_its = 0;
            msg_active = false;
            msg.free();
        }
    }
}

void Character::render() const {
    if (!state) {
        throw Exception(
            "Character has not been initialized (render requested).");
    }

    // Armadura / cuerpo (si no tiene armadura)
    if (equipment[ARMOUR]) {
        _render(g_sprites->get(equipment[ARMOUR], is_shorter));
    } else if (body_id) {
        _render(g_sprites->get(body_id));
    }

    // Cabeza
    if (head_id) {
        _render(g_sprites->get(head_id));
    }

    // Casco
    if (equipment[HELMET]) {
        _render(g_sprites->get(equipment[HELMET], is_shorter));
    }

    // Espada
    if (equipment[WEAPON]) {
        _render(g_sprites->get(equipment[WEAPON], is_shorter));
    }

    // Escudo
    if (equipment[SHIELD]) {
        _render(g_sprites->get(equipment[SHIELD], is_shorter));
    }

    // Info
    _renderInfo();
}

Character::~Character() {}

//-----------------------------------------------------------------------------
