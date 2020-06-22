#include "../../includes/View/UserInventory.h"

//-----------------------------------------------------------------------------
// Métodos privados

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
        config["components"]["equipment"]["weapon_offset"]["x"];
    equipment_weapon.x += render_rect.x;
    equipment_weapon.y =
        config["components"]["equipment"]["weapon_offset"]["y"];
    equipment_weapon.y += render_rect.y;

    equipment_helmet.x =
        config["components"]["equipment"]["helmet_offset"]["x"];
    equipment_helmet.x += render_rect.x;
    equipment_helmet.y =
        config["components"]["equipment"]["helmet_offset"]["y"];
    equipment_helmet.y += render_rect.y;

    equipment_armour.x =
        config["components"]["equipment"]["armour_offset"]["x"];
    equipment_armour.x += render_rect.x;
    equipment_armour.y =
        config["components"]["equipment"]["armour_offset"]["y"];
    equipment_armour.y += render_rect.y;

    equipment_shield.x =
        config["components"]["equipment"]["shield_offset"]["x"];
    equipment_shield.x += render_rect.x;
    equipment_shield.y =
        config["components"]["equipment"]["shield_offset"]["y"];
    equipment_shield.y += render_rect.y;

    // Inventario

    initialized = true;
}

void UserInventory::loadMedia() {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INVENTORY_BASE_FP);
}

void UserInventory::update() {
    equipment = g_player.getEquipment();
    // const InventoryData& inventory = g_player.getInventory();
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
}

void UserInventory::free() {}

UserInventory::~UserInventory() {
    free();
}

//-----------------------------------------------------------------------------
