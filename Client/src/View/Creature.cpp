#include "../../includes/View/Creature.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Creature::_copyData(const CreatureData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Data del creature
    name = init_data.name;
    creature_id = init_data.creature_id;
    health = init_data.health;
    // level = init_data.level;
}

void Creature::_renderInfo() const {
    // Primero el nivel
    SDL_Rect level_quad = {0, 0, info_level.getWidth(), info_level.getHeight()};

    // Centramos las coordenadas
    level_quad.x = (TILE_WIDTH - level_quad.w) / 2;
    level_quad.y = (TILE_HEIGHT * (0.8)) - level_quad.h -
                   g_sprites->get(creature_id).clip_h - INFO_SPACE_FROM_HEAD;

    // Le agregamos offsets de la unidad
    level_quad.x += (int)this->x;
    level_quad.y += (int)this->y;

    SDL_Rect nick_quad = level_quad;

    g_renderer->renderIfVisible(info_level.getTexture(), &level_quad);

    // Ahora el nickname
    nick_quad.w = info_nickname.getWidth();
    nick_quad.h = info_nickname.getHeight();
    nick_quad.x = (TILE_WIDTH - nick_quad.w) / 2;
    nick_quad.x += (int)this->x;
    nick_quad.y -= nick_quad.h;

    // Sombra
    SDL_Rect nick_quad_bg = nick_quad;
    nick_quad_bg.y++;

    g_renderer->renderIfVisible(info_nickname_shadow.getTexture(),
                                &nick_quad_bg);
    g_renderer->renderIfVisible(info_nickname.getTexture(), &nick_quad);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Creature::Creature(Renderer* renderer, UnitSpriteContainer* sprites,
                   TTF_Font* g_nickname_font, TTF_Font* g_level_font)
    : Unit(renderer, sprites),
      g_nickname_font(g_nickname_font),
      g_level_font(g_level_font) {}

void Creature::init(const CreatureData& init_data) {
    if (state) {
        throw Exception("Creature has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Cargamos la info */
    if (!g_nickname_font || !g_level_font) {
        throw Exception("Player::init: Fonts not initialized.");
    }

    info_nickname.loadFromRenderedText(g_renderer, g_nickname_font, name,
                                       CREATURE_NICKNAME_COLOR);
    info_nickname_shadow.loadFromRenderedText(g_renderer, g_nickname_font, name,
                                              SDL_Color({0, 0, 0, 255}));
    info_level.loadFromRenderedText(g_renderer, g_level_font,
                                    "Nivel " + std::to_string(99));

    /* Seteamos nuestra posición en pixeles para el renderizado */
    x = TILE_WIDTH * data.x_tile;
    y = TILE_HEIGHT * data.y_tile;

    /* Completamos la inicialización */
    state = READY;
}

void Creature::update(const CreatureData& updated_data) {
    if (!state) {
        throw Exception(
            "Creature has not been initialized (update requested).");
    }

    /* Verificamos si hay que modificar la info */
    /*
    if (level != updated_data.level) {
        info_level.loadFromRenderedText(g_renderer, g_level_font,
                                        "Nivel " + std::to_string(level));
    }
    */

    /* Actualizamos la data */
    _copyData(updated_data);

    /* Iniciamos el movimiento si nuestra posición en pixeles no coincide*/
    _setMovementSpeed();
}

void Creature::render() const {
    if (!state) {
        throw Exception(
            "Creature has not been initialized (render requested).");
    }

    // Cuerpo
    _render(g_sprites->get(creature_id));

    // Info
    _renderInfo();
}

Creature::~Creature() {}

//-----------------------------------------------------------------------------
