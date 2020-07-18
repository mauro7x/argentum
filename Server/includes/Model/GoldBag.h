#ifndef __GOLD_BAG_H__
#define __GOLD_BAG_H__
//-----------------------------------------------------------------------------
#include "Item.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

class GoldBag : public Item {
   public:
    GoldBag(const OtherItemsCfg& data);
    ~GoldBag();

    /*
     * Lanza excepción, dado que la bolsa de
     * oro no se debería equipar.
     */
    Response equip(Character& equipper) override;
};

//-----------------------------------------------------------------------------
#endif  // __GOLD_BAG_H__
//-----------------------------------------------------------------------------
