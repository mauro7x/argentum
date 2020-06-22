#include "../../includes/View/Player.h"

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

//-----------------------------------------------------------------------------
// API Pública

Player::Player(Renderer* renderer, UnitSpriteContainer* sprites)
    : Unit(renderer, sprites) {}

void Player::init(const PlayerData& init_data) {
    if (state) {
        throw Exception("Player has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Cargamos dimensiones del tile */
    json map_data = JSON::loadJsonFile(MAPS_FILEPATH);
    tile_w = map_data["tilewidth"];
    tile_h = map_data["tileheight"];

    /* Con ellas, seteamos nuestra posición en pixeles para el renderizado */
    x = tile_w * data.x_tile;
    y = tile_h * data.y_tile;

    /* Cargamos velocidad */
    json common_config = JSON::loadJsonFile(COMMON_CONFIG_FILEPATH);
    int speed = common_config["tiles_per_sec"]["character_speed"]; /* tiles/s */
    tile_movement_time = 1000 / speed;                             /* ms */

    /* Completamos la inicialización */
    state = READY;
}

void Player::update(const PlayerData& updated_data) {
    if (!state) {
        throw Exception("Player has not been initialized (update requested).");
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

Player::~Player() {}

//-----------------------------------------------------------------------------
