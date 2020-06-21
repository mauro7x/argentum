#include "../includes/json_conversion.h"

void to_json(json& j, const InventorySlot& data) {
    j = json{{"item", data.item}, 
             {"amount", data.amount}};
}

void from_json(const json& j, InventorySlot& data) {
    j.at("item").get_to(data.item);
    j.at("amount").get_to(data.amount);
}

void to_json(json& j, const PlayerData& data) {
    j = json{{"gid", data.basic_data.gid}, 
             {"map", data.basic_data.map}, 
             {"x_tile", data.basic_data.x_tile},
             {"y_tile", data.basic_data.y_tile},
             {"orientation", data.basic_data.orientation},
             {"nickname", data.nickname},
             {"head_id", data.head_id},
             {"body_id", data.body_id},
             {"health", data.health},
             {"max_health", data.max_health},
             {"mana", data.mana},
             {"max_mana", data.max_mana},
             {"safe_gold", data.safe_gold},
             {"excess_gold", data.excess_gold},
             {"level", data.level},
             {"exp", data.exp},
             {"levelup_exp", data.levelup_exp},
             };
    j["equipment"] = json(data.equipment);
    j["inventory"] = json(data.inventory);
}

void from_json(const json& j, PlayerData& data) {
    j.at("gid").get_to(data.basic_data.gid);
    j.at("map").get_to(data.basic_data.map);
    j.at("map").get_to(data.basic_data.map); 
    j.at("x_tile").get_to(data.basic_data.x_tile);
    j.at("y_tile").get_to(data.basic_data.y_tile);
    j.at("orientation").get_to(data.basic_data.orientation);
    j.at("nickname").get_to(data.nickname);
    j.at("head_id").get_to(data.head_id);
    j.at("body_id").get_to(data.body_id);
    j.at("health").get_to(data.health);
    j.at("max_health").get_to(data.max_health);
    j.at("mana").get_to(data.mana);
    j.at("max_mana").get_to(data.max_mana);
    j.at("safe_gold").get_to(data.safe_gold);
    j.at("excess_gold").get_to(data.excess_gold);
    j.at("level").get_to(data.level);
    j.at("exp").get_to(data.exp);
    j.at("levelup_exp").get_to(data.levelup_exp);
    data.equipment = j.at("equipment").get<EquipmentData>();
    data.inventory = j.at("inventory").get<InventoryData>();
}
