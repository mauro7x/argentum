#ifndef __DEFENCE_H__
#define __DEFENCE_H__

#include "Wearable.h"
#include "config_structs.h"

class Defence: public Wearable {
    public:
        Defence(DefenceCfg& data);
        ~Defence();

        Defence(const Defence&) = delete;
        Defence& operator=(const Defence&) = delete;
        Defence(Defence&&) = delete;
        Defence& operator=(Defence&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item y Wearable.

        virtual const unsigned int use() override;
};

#endif
