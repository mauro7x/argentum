#ifndef __JSON_CONVERSION_H__
#define __JSON_CONVERSION_H__

#include "json.hpp"
#include "UnitData.h"

using json = nlohmann::json;

void to_json(json& j, const InventorySlot& data);
void to_json(json& j, const PlayerData& data);
void to_json(json& j, const CreatureData& data);

void from_json(const json& j, InventorySlot& data);
void from_json(const json& j, PlayerData& data);
void from_json(const json& j, CreatureData& data);

#endif // __JSON_CONVERSION_H__
