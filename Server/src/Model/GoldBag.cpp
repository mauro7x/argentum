#include "../../includes/Model/GoldBag.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------

GoldBag::GoldBag(const OtherItemsCfg& data) : Item(data.id, data.name, 0) {}
GoldBag::~GoldBag() {}

Response GoldBag::equip(Character& equipper) {
    throw Exception("No se puede equipar una bolsa de oro.");
}

//-----------------------------------------------------------------------------
