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
    max_health = init_data.max_health;
}

void Creature::_renderInfo() const {
    // Primero la barra de vida
    float relative_progress = ((float)health) / max_health;
    SDL_Rect health_bar_bg = {0, 0, CREATURE_HEALTH_BAR_W,
                              CREATURE_HEALTH_BAR_H};

    // Centramos las coordenadas
    health_bar_bg.x = (TILE_WIDTH - health_bar_bg.w) / 2;
    health_bar_bg.y = (TILE_HEIGHT * (0.8)) - health_bar_bg.h -
                      g_sprites->get(creature_id).clip_h - INFO_SPACE_FROM_HEAD;

    // Le agregamos offsets de la unidad
    health_bar_bg.x += (int)this->x;
    health_bar_bg.y += (int)this->y;

    SDL_Rect health_bar = health_bar_bg;
    health_bar.w *= relative_progress;
    health_bar.x += 1;
    health_bar.y += 1;
    health_bar.w -= 2;
    health_bar.h -= 2;

    SDL_Rect nick_quad = health_bar_bg;

    // renderizar aca las barras
    g_renderer->fillQuadIfVisible(&health_bar_bg);
    g_renderer->fillQuadIfVisible(&health_bar, CREATURE_HEALTH_BAR_COLOR);

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
    : Unit(renderer, sprites), g_nickname_font(g_nickname_font) {}

void Creature::init(const CreatureData& init_data) {
    if (state) {
        throw Exception("Creature has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Cargamos la info */
    if (!g_nickname_font) {
        throw Exception("Player::init: Fonts not initialized.");
    }

    info_nickname.loadFromRenderedText(g_renderer, g_nickname_font, name,
                                       CREATURE_NICKNAME_COLOR);
    info_nickname_shadow.loadFromRenderedText(g_renderer, g_nickname_font, name,
                                              SDL_Color({0, 0, 0, 255}));

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
