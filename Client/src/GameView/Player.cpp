#include "../../includes/GameView/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_copyData(const PlayerData& init_data) {
    // Data básica
    data = init_data.basic_data;
    nickname = init_data.nickname;

    // Ids gráficos
    head_id = init_data.head_id;
    body_id = init_data.body_id;
    equipment = init_data.equipment;
    inventory = init_data.inventory;

    // Stats
    health = init_data.health;
    max_health = init_data.max_health;
    mana = init_data.mana;
    max_mana = init_data.max_mana;
    safe_gold = init_data.safe_gold;
    excess_gold = init_data.excess_gold;
    level = init_data.level;
    exp = init_data.exp;
    levelup_exp = init_data.levelup_exp;
}

void Player::_renderInfo() const {
    // Primero el nivel
    SDL_Rect level_quad = {0, 0, info_level.getWidth(), info_level.getHeight()};

    // Centramos las coordenadas
    level_quad.x = (TILE_WIDTH - level_quad.w) / 2;
    level_quad.y = (TILE_HEIGHT * (0.8)) - level_quad.h -
                   g_sprites->get(head_id).clip_h - INFO_SPACE_FROM_HEAD;

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
// API Pública

Player::Player(Renderer* renderer, const Camera& camera,
               UnitSpriteContainer* sprites)
    : Unit(renderer, camera, sprites) {}

void Player::loadMedia() {
    // Fuentes a utilizar
    nickname_font = TTF_OpenFont(FONT_AUGUSTA_FP, INFO_NAME_FONTSIZE);
    level_font = TTF_OpenFont(FONT_CINZELBOLD_FP, INFO_LVL_FONTSIZE);

    if (!nickname_font || !level_font) {
        throw Exception("Player::loadMedia: Error opening TTF_Font/s.");
    }
}

void Player::init(const PlayerData& init_data) {
    if (state) {
        throw Exception("Player has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Con ellas, seteamos nuestra posición en pixeles para el renderizado */
    x = TILE_WIDTH * data.x_tile;
    y = TILE_HEIGHT * data.y_tile;

    /* Cargamos la info */
    if (!nickname_font || !level_font) {
        throw Exception("Player::init: Fonts not initialized.");
    }

    info_nickname.loadFromRenderedText(g_renderer, nickname_font, nickname,
                                       PLAYER_NICKNAME_COLOR);
    info_nickname_shadow.loadFromRenderedText(
        g_renderer, nickname_font, nickname, SDL_Color({0, 0, 0, 255}));
    info_level.loadFromRenderedText(g_renderer, level_font,
                                    "Nivel " + std::to_string(level));

    /* Completamos la inicialización */
    state = READY;
}

void Player::update(const PlayerData& updated_data) {
    if (!state) {
        throw Exception("Player has not been initialized (update requested).");
    }

    /* Verificamos si hay que modificar la info */
    if (level != updated_data.level) {
        info_level.loadFromRenderedText(g_renderer, level_font,
                                        "Nivel " + std::to_string(level));
    }

    /* Actualizamos la data */
    _copyData(updated_data);

    /* Iniciamos el movimiento si nuestra posición en pixeles no coincide*/
    _setMovementSpeed();
}

void Player::render() const {
    if (!state) {
        throw Exception("Player has not been initialized (render requested).");
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

SDL_Rect Player::getBox() const {
    if (!state) {
        throw Exception("Player has not been initialized (box requested).");
    }

    int body_w = g_sprites->get(body_id).clip_w;
    int head_h = g_sprites->get(head_id).clip_h;
    return SDL_Rect({(int)x, (int)y, body_w, head_h});
}

//-------------------------------------------------------------------------
// Getters para graficar en el HUD

const std::string& Player::getNickname() const {
    return nickname;
}

const EquipmentData& Player::getEquipment() const {
    return equipment;
}

const InventoryData& Player::getInventory() const {
    return inventory;
}

const uint32_t& Player::getHealth() const {
    return health;
}

const uint32_t& Player::getMaxHealth() const {
    return max_health;
}

const uint32_t& Player::getMana() const {
    return mana;
}

const uint32_t& Player::getMaxMana() const {
    return max_mana;
}

const uint32_t& Player::getSafeGold() const {
    return safe_gold;
}

const uint32_t& Player::getExcessGold() const {
    return excess_gold;
}

const uint32_t& Player::getLevel() const {
    return level;
}

const uint32_t& Player::getExp() const {
    return exp;
}

const uint32_t& Player::getLvlUpExp() const {
    return levelup_exp;
}

//-------------------------------------------------------------------------

Player::~Player() {
    if (nickname_font) {
        TTF_CloseFont(nickname_font);
        nickname_font = NULL;
    }

    if (level_font) {
        TTF_CloseFont(level_font);
        level_font = NULL;
    }
}

//-----------------------------------------------------------------------------
