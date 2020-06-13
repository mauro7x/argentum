#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_copyData(const PlayerData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Data específica
    health = init_data.health;
    mana = init_data.mana;
    gold = init_data.gold;

    // Ids gráficos
    head_id = init_data.head_id;
    body_id = init_data.body_id;
    helmet_id = init_data.helmet_id;
    armour_id = init_data.armour_id;
    shield_id = init_data.shield_id;
    weapon_id = init_data.weapon_id;
}

//-----------------------------------------------------------------------------
// API Pública

Player::Player(Renderer* renderer, UnitSpriteContainer* sprites)
    : Unit(renderer, sprites),
      health(0),
      mana(0),
      gold(0),
      head_id(0),
      body_id(0),
      helmet_id(0),
      armour_id(0),
      shield_id(0),
      weapon_id(0) {}

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
    if (armour_id) {
        _render(g_sprites->get(armour_id));
    }

    // Escudo
    if (shield_id) {
        _render(g_sprites->get(shield_id));
    }

    // Espada
    if (weapon_id) {
        _render(g_sprites->get(weapon_id));
    }

    // Cabeza
    if (head_id) {
        _render(g_sprites->get(head_id));
    }

    // Casco
    if (helmet_id) {
        _render(g_sprites->get(helmet_id));
    }
}

SDL_Rect Player::getPos() const {
    if (!state) {
        throw Exception("Player has not been initialized (pos requested).");
    }

    return SDL_Rect({data.x_tile, data.y_tile, 0, 0});
}

SDL_Rect Player::getBox() const {
    if (!state) {
        throw Exception("Player has not been initialized (box requested).");
    }

    int body_w = g_sprites->get(body_id).clip_w;
    int head_h = g_sprites->get(head_id).clip_h;
    return SDL_Rect({(int)x, (int)y, body_w, head_h});
}

Player::~Player() {}

//-----------------------------------------------------------------------------
