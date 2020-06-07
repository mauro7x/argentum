#ifndef __DEFENCE_H__
#define __DEFENCE_H__

#include "Wearable.h"

class Defence: public Wearable {
    public:
        Defence(const unsigned int id, const unsigned int price, 
               WearableType type,
               const unsigned int min_defence, 
               const unsigned int max_defence);
        ~Defence();

        Defence(const Defence&) = delete;
        Defence& operator=(const Defence&) = delete;
        Defence(Defence&&) = delete;
        Defence& operator=(Defence&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item y Wearable.
        
        virtual void equip(Character& character) override;
        virtual const unsigned int use() override;
};

#endif
