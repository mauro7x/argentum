#include "../../includes/GameView/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_copyData(const PlayerData& data) {
    // Data básica
    this->data = data.basic_data;
    this->nickname = data.nickname;

    // Ids gráficos
    this->head_id = data.head_id;
    this->body_id = data.body_id;
    this->equipment = data.equipment;
    this->inventory = data.inventory;

    // Stats
    this->health = data.health;
    this->max_health = data.max_health;
    this->mana = data.mana;
    this->max_mana = data.max_mana;
    this->safe_gold = data.safe_gold;
    this->excess_gold = data.excess_gold;
    this->level = data.level;
    this->exp = data.exp;
    this->levelup_exp = data.levelup_exp;
    this->prev_levelup_exp = data.prev_levelup_exp;

    // Si es enano/gnomo
    this->is_shorter = data.is_shorter;
}

void Player::_renderInfo() const {
    /*
    // Primero el nickname
    SDL_Rect nick_quad = {0, 0, info_nickname.getWidth(),
                          info_nickname.getHeight()};

    // Centramos las coordenadas
    nick_quad.x = (TILE_WIDTH - nick_quad.w) / 2;
    nick_quad.y = TILE_HEIGHT;

    // Le agregamos offsets de la unidad
    nick_quad.x += (int)this->x;
    nick_quad.y += (int)this->y;

    // Lo copiamos para el nivel
    SDL_Rect level_quad = nick_quad;

    // Somrba
    SDL_Rect nick_quad_bg = nick_quad;
    nick_quad_bg.y++;

    g_camera.renderIfVisible(g_renderer, info_nickname_shadow.getTexture(),
                             nick_quad_bg);
    g_camera.renderIfVisible(g_renderer, info_nickname.getTexture(), nick_quad);

    // Ahora el nivel
    level_quad.w = info_level.getWidth();
    level_quad.h = info_level.getHeight();
    level_quad.x = (TILE_WIDTH - level_quad.w) / 2;
    level_quad.x += (int)this->x;
    level_quad.y += nick_quad.h;

    g_camera.renderIfVisible(g_renderer, info_level.getTexture(), level_quad);
    */

    /*
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
    */
}

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, const Camera& camera,
               UnitSpriteContainer* sprites)
    : Unit(renderer, camera, sprites) {}

void Player::loadMedia() {
    // Fuentes a utilizar
    nickname_font =
        TTF_OpenFont(paths::asset(FONT_AUGUSTA_FP).c_str(), INFO_NAME_FONTSIZE);
    level_font = TTF_OpenFont(paths::asset(FONT_CINZELBOLD_FP).c_str(),
                              INFO_LVL_FONTSIZE);

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
        info_level.loadFromRenderedText(
            g_renderer, level_font,
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

void Player::addMessage(InstanceId sender_id, const std::string& msg) {
    if (sender_id != data.gid) {
        // el mensaje no es nuestro
        return;
    }

    // actualizar el mensaje
    fprintf(stderr, "llego mensaje del player.\n");
}

void Player::render() const {
    if (!state) {
        throw Exception("Player has not been initialized (render requested).");
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

    // Escudo
    if (equipment[SHIELD]) {
        _render(g_sprites->get(equipment[SHIELD], is_shorter));
    }

    // Espada
    if (equipment[WEAPON]) {
        _render(g_sprites->get(equipment[WEAPON], is_shorter));
    }

    // Info
    _renderInfo();
}

SDL_Rect Player::getBox() const {
    if (!state) {
        throw Exception("Player has not been initialized (box requested).");
    }

    int body_w = g_sprites->get(body_id).clip_w;
    int head_h;

    if (head_id) {
        head_h = g_sprites->get(head_id).clip_h;
    } else {
        // en caso de que estemos muertos
        head_h = g_sprites->get(body_id).clip_h;
    }

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

const uint32_t& Player::getPrevLvlUpExp() const {
    return prev_levelup_exp;
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
