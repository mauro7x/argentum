#include "../../includes/View/UserInventory.h"

//-----------------------------------------------------------------------------
// Métodos privados

void UserInventory::_center(SDL_Point& texture_pos, const Texture& texture,
                            const SDL_Rect& rect) {
    texture_pos.x = rect.x + (rect.w - texture.getWidth()) / 2;
    texture_pos.y = rect.y + (rect.h - texture.getHeight()) / 2;
}

void UserInventory::_renderEquipment() const {
    SDL_Rect render_quad;

    // Weapon
    if (equipment[WEAPON]) {
        render_quad = {equipment_weapon.x, equipment_weapon.y, slot_w, slot_h};
        const Texture& texture = g_item_sprites[equipment[WEAPON]].texture;
        g_renderer->render(texture.getTexture(), &render_quad);
    }

    // Helmet
    if (equipment[HELMET]) {
        render_quad = {equipment_helmet.x, equipment_helmet.y, slot_w, slot_h};
        const Texture& texture = g_item_sprites[equipment[HELMET]].texture;
        g_renderer->render(texture.getTexture(), &render_quad);
    }

    // Armour
    if (equipment[ARMOUR]) {
        render_quad = {equipment_armour.x, equipment_armour.y, slot_w, slot_h};
        const Texture& texture = g_item_sprites[equipment[ARMOUR]].texture;
        g_renderer->render(texture.getTexture(), &render_quad);
    }

    // Shield
    if (equipment[SHIELD]) {
        render_quad = {equipment_shield.x, equipment_shield.y, slot_w, slot_h};
        const Texture& texture = g_item_sprites[equipment[SHIELD]].texture;
        g_renderer->render(texture.getTexture(), &render_quad);
    }
}

void UserInventory::_renderInventory() const {
    SDL_Rect render_quad = {0};
    int slot_index = 0;

    for (int row = 0; row < inventory_rows; row++) {
        for (int col = 0; col < slots_per_row; col++) {
            slot_index = row * slots_per_row + col;
            if (inventory[slot_index].item) {
                // Graficamos el item
                render_quad = {
                    first_inventory_slot.x + (col * offset_between_slots.x),
                    first_inventory_slot.y + (row * offset_between_slots.y),
                    slot_w, slot_h};
                const Texture& item_texture =
                    g_item_sprites[inventory[slot_index].item].texture;
                g_renderer->render(item_texture.getTexture(), &render_quad);

                // Graficamos la cantidad (corremos un poco manualmente el
                // render_rect para que quede mas esquinado)
                const Texture& amount_texture =
                    inventory_quantities[slot_index];
                render_quad = {
                    first_inventory_slot.x + (col * offset_between_slots.x),
                    first_inventory_slot.y + (row * offset_between_slots.y) - 2,
                    amount_texture.getWidth(), amount_texture.getHeight()};
                g_renderer->render(amount_texture.getTexture(), &render_quad);
            }
        }
    }
}

void UserInventory::_renderGold() const {
    SDL_Rect render_quad = {safe_gold_pos.x, safe_gold_pos.y,
                            safe_gold.getWidth(), safe_gold.getHeight()};
    g_renderer->render(safe_gold.getTexture(), &render_quad);

    render_quad = {excess_gold_pos.x, excess_gold_pos.y, excess_gold.getWidth(),
                   excess_gold.getHeight()};
    g_renderer->render(excess_gold.getTexture(), &render_quad);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserInventory::UserInventory(const Renderer* renderer,
                             const ItemSpriteContainer& item_sprites,
                             const Player& player)
    : HUDComponent(renderer), g_item_sprites(item_sprites), g_player(player) {}

void UserInventory::init(const json& config) {
    if (initialized) {
        throw Exception("UserInventory already initialized.");
    }

    // Offsets de renderizado
    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    // Dimensiones de los slots
    slot_w = config["components"]["slot_w"];
    slot_h = config["components"]["slot_h"];

    // Equipamiento
    equipment_weapon.x =
        (int)config["components"]["equipment"]["weapon_offset"]["x"] +
        render_rect.x;
    equipment_weapon.y =
        (int)config["components"]["equipment"]["weapon_offset"]["y"] +
        render_rect.y;

    equipment_helmet.x =
        (int)config["components"]["equipment"]["helmet_offset"]["x"] +
        render_rect.x;
    equipment_helmet.y =
        (int)config["components"]["equipment"]["helmet_offset"]["y"] +
        render_rect.y;

    equipment_armour.x =
        (int)config["components"]["equipment"]["armour_offset"]["x"] +
        render_rect.x;
    equipment_armour.y =
        (int)config["components"]["equipment"]["armour_offset"]["y"] +
        render_rect.y;

    equipment_shield.x =
        (int)config["components"]["equipment"]["shield_offset"]["x"] +
        render_rect.x;
    equipment_shield.y =
        (int)config["components"]["equipment"]["shield_offset"]["y"] +
        render_rect.y;

    // Inventario
    quantities_fontsize =
        config["components"]["inventory"]["quantities_fontsize"];
    first_inventory_slot.x =
        (int)config["components"]["inventory"]["first_slot_offset"]["x"] +
        render_rect.x;
    first_inventory_slot.y =
        (int)config["components"]["inventory"]["first_slot_offset"]["y"] +
        render_rect.y;
    inventory_rows = config["components"]["inventory"]["rows"];
    slots_per_row = config["components"]["inventory"]["slots_per_row"];
    offset_between_slots.x =
        config["components"]["inventory"]["offset_between_slots"]["x"];
    offset_between_slots.y =
        config["components"]["inventory"]["offset_between_slots"]["y"];

    // Oro seguro
    gold_fontsize = config["components"]["gold"]["fontsize"];
    safe_gold_box.x =
        (int)config["components"]["gold"]["safe_gold_offset"]["x"] +
        render_rect.x;
    safe_gold_box.y =
        (int)config["components"]["gold"]["safe_gold_offset"]["y"] +
        render_rect.y;
    safe_gold_box.w = (int)config["components"]["gold"]["w"];
    safe_gold_box.h = (int)config["components"]["gold"]["h"];

    // Oro en exceso
    excess_gold_box.x =
        (int)config["components"]["gold"]["excess_gold_offset"]["x"] +
        render_rect.x;
    excess_gold_box.y =
        (int)config["components"]["gold"]["excess_gold_offset"]["y"] +
        render_rect.y;
    excess_gold_box.w = (int)config["components"]["gold"]["w"];
    excess_gold_box.h = (int)config["components"]["gold"]["h"];

    initialized = true;
}

void UserInventory::loadMedia() {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INVENTORY_BASE_FP);

    // Fuentes a utilizar
    quantities_font = TTF_OpenFont(FONT_CINZELBOLD_FP, quantities_fontsize);
    gold_font = TTF_OpenFont(FONT_AUGUSTA_FP, gold_fontsize);

    if (!quantities_font || !gold_font) {
        throw Exception("UserInventory::loadMedia: Error opening TTF_Font/s.");
    }

    uint32_t quantity = 0;
    for (int i = 0; i < N_INVENTORY_SLOTS; i++) {
        quantity = inventory[i].amount;
        inventory_quantities[i].loadFromRenderedText(
            g_renderer, quantities_font, " " + std::to_string(quantity) + " ",
            SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    }

    safe_gold.loadFromRenderedText(g_renderer, gold_font,
                                   std::to_string(current_safe_gold),
                                   SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(safe_gold_pos, safe_gold, safe_gold_box);

    excess_gold.loadFromRenderedText(g_renderer, gold_font,
                                     std::to_string(current_excess_gold),
                                     SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(excess_gold_pos, excess_gold, excess_gold_box);
}

void UserInventory::update() {
    // Actualizamos el equipamiento
    equipment = g_player.getEquipment();

    // Actualizamos el inventario
    const InventoryData& updated_inventory = g_player.getInventory();
    for (int i = 0; i < N_INVENTORY_SLOTS; i++) {
        if ((inventory[i].item != updated_inventory[i].item) ||
            (inventory[i].amount != updated_inventory[i].amount)) {
            inventory[i] = updated_inventory[i];
            inventory_quantities[i].loadFromRenderedText(
                g_renderer, quantities_font,
                " " + std::to_string(inventory[i].amount) + " ",
                SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        }
    }

    // Actualizamos el oro si es necesario
    int updated_safe_gold = g_player.getSafeGold();
    if (current_safe_gold != updated_safe_gold) {
        current_safe_gold = updated_safe_gold;
        safe_gold.loadFromRenderedText(g_renderer, gold_font,
                                       std::to_string(current_safe_gold),
                                       SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        _center(safe_gold_pos, safe_gold, safe_gold_box);
    }

    int updated_excess_gold = g_player.getExcessGold();
    if (current_excess_gold != updated_excess_gold) {
        current_excess_gold = updated_excess_gold;
        excess_gold.loadFromRenderedText(g_renderer, gold_font,
                                         std::to_string(current_excess_gold),
                                         SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
        _center(excess_gold_pos, excess_gold, excess_gold_box);
    }
}

void UserInventory::render() const {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);

    // Renderizar el equipamiento
    _renderEquipment();

    // Renderizar el inventario
    _renderInventory();

    // Renderizar el orto
    _renderGold();
}

void UserInventory::free() {
    if (quantities_font) {
        TTF_CloseFont(quantities_font);
        quantities_font = NULL;
    }
}

UserInventory::~UserInventory() {
    free();
}

//-----------------------------------------------------------------------------
