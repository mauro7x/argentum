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

    for (int i = 0; i < N_WEARABLE_ITEMS; i++) {
        if (equipment[i]) {
            render_quad = {equipment_slots[i].x, equipment_slots[i].y, slot_w,
                           slot_h};
            const Texture& texture = g_item_sprites[equipment[i]].texture;
            g_renderer->render(texture.getTexture(), &render_quad);
        }
    }
}

void UserInventory::_renderInventory() const {
    SDL_Rect render_quad = {0};
    /*
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
    */

    for (int i = 0; i < N_INVENTORY_SLOTS; i++) {
        if (inventory[i].item) {
            // Graficamos el item
            render_quad = {inventory_slots[i].x, inventory_slots[i].y, slot_w,
                           slot_h};
            const Texture& item_texture =
                g_item_sprites[inventory[i].item].texture;
            g_renderer->render(item_texture.getTexture(), &render_quad);

            // Graficamos la cantidad (corremos un poco manualmente el
            // render_rect para que quede mas esquinado)
            const Texture& amount_texture = inventory_quantities[i];
            render_quad = {inventory_slots[i].x, inventory_slots[i].y - 2,
                           amount_texture.getWidth(),
                           amount_texture.getHeight()};
            g_renderer->render(amount_texture.getTexture(), &render_quad);
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

bool UserInventory::_wasClicked(const SDL_Point& click_pos,
                                const SDL_Point& slot_pos) const {
    if ((click_pos.x > (slot_pos.x + slot_w)) || (click_pos.x < (slot_pos.x)) ||
        (click_pos.y > (slot_pos.y + slot_h)) || (click_pos.y < (slot_pos.y))) {
        return false;
    }

    return true;
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

    // Cargamos el tamaño de fuente del inventario
    quantities_fontsize =
        config["components"]["inventory"]["quantities_fontsize"];

    // Cargamos los offsets de renderizado del equipamiento
    {
        SDL_Point current_pos;
        current_pos.x =
            render_rect.x +
            (int)config["components"]["equipment"]["first_slot_offset"]["x"];
        current_pos.y =
            render_rect.y +
            (int)config["components"]["equipment"]["first_slot_offset"]["y"];
        int x_offset_between_slots =
            (int)config["components"]["equipment"]["x_offset_between_slots"];

        for (int i = 0; i < N_WEARABLE_ITEMS; i++) {
            equipment_slots[i].x = current_pos.x;
            equipment_slots[i].y = current_pos.y;
            current_pos.x += x_offset_between_slots;
        }
    }

    // Cargamos los offsets de renderizado del inventario
    {
        SDL_Point first_pos;
        first_pos.x =
            render_rect.x +
            (int)config["components"]["inventory"]["first_slot_offset"]["x"];
        first_pos.y =
            render_rect.y +
            (int)config["components"]["inventory"]["first_slot_offset"]["y"];
        int x_offset_between_slots =
            (int)config["components"]["inventory"]["offset_between_slots"]["x"];
        int y_offset_between_slots =
            (int)config["components"]["inventory"]["offset_between_slots"]["y"];
        int inventory_rows = (int)config["components"]["inventory"]["rows"];
        int slots_per_row =
            (int)config["components"]["inventory"]["slots_per_row"];

        int slot_index = 0;
        for (int row = 0; row < inventory_rows; row++) {
            for (int col = 0; col < slots_per_row; col++) {
                slot_index = (row * slots_per_row) + col;
                inventory_slots[slot_index].x =
                    first_pos.x + (col * x_offset_between_slots);
                inventory_slots[slot_index].y =
                    first_pos.y + (row * y_offset_between_slots);
            }
        }
    }

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
            g_renderer, quantities_font, " " + std::to_string(quantity) + " ");
    }

    safe_gold.loadFromRenderedText(g_renderer, gold_font,
                                   std::to_string(current_safe_gold));
    _center(safe_gold_pos, safe_gold, safe_gold_box);

    excess_gold.loadFromRenderedText(g_renderer, gold_font,
                                     std::to_string(current_excess_gold));
    _center(excess_gold_pos, excess_gold, excess_gold_box);
}

int8_t UserInventory::getEquipmentSlotClicked(const SDL_Point& click_pos) {
    for (int8_t i = 0; i < N_WEARABLE_ITEMS; i++) {
        if (_wasClicked(click_pos, equipment_slots[i])) {
            fprintf(stderr, "El slot %i del equipamiento fue clickeado.\n", i);
            return i;
        }
    }

    return -1;
}

int8_t UserInventory::getInventorySlotClicked(const SDL_Point& click_pos) {
    for (int8_t i = 0; i < N_INVENTORY_SLOTS; i++) {
        if (_wasClicked(click_pos, inventory_slots[i])) {
            fprintf(stderr, "El slot %i del inventario fue clickeado.\n", i);
            return i;
        }
    }

    return -1;
}

void UserInventory::update(const int it) {
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
                " " + std::to_string(inventory[i].amount) + " ");
        }
    }

    // Actualizamos el oro si es necesario
    int updated_safe_gold = g_player.getSafeGold();
    if (current_safe_gold != updated_safe_gold) {
        current_safe_gold = updated_safe_gold;
        safe_gold.loadFromRenderedText(g_renderer, gold_font,
                                       std::to_string(current_safe_gold));
        _center(safe_gold_pos, safe_gold, safe_gold_box);
    }

    int updated_excess_gold = g_player.getExcessGold();
    if (current_excess_gold != updated_excess_gold) {
        current_excess_gold = updated_excess_gold;
        excess_gold.loadFromRenderedText(g_renderer, gold_font,
                                         std::to_string(current_excess_gold));
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
