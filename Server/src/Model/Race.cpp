#include "../../includes/Model/Race.h"

Race::Race(const RaceCfg& data, Id head_id, Id body_id)
    : id(data.id),
      name(data.name),
      head_id(head_id),
      body_id(body_id),
      dead_head_id(data.dead_head_id),
      dead_body_id(data.dead_body_id),
      max_health_factor(data.max_health_factor),
      max_mana_factor(data.max_mana_factor),
      health_recovery_factor(data.health_recovery_factor),
      mana_recovery_factor(data.mana_recovery_factor),
      is_shorter(data.is_shorter) {}

Race::~Race() {}
