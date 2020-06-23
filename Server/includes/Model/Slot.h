#ifndef __SLOT_H__
#define __SLOT_H__

#include "../../../Common/includes/types.h"
#include "Item.h"

class Slot {
    private:
        Item* item;
        uint32_t amount;

    public:
        Slot();
        Slot(Item* item, uint32_t amount);
        ~Slot();

        Slot(Slot&& other);
        Slot& operator=(Slot&& other);

        Slot(const Slot&) = delete;
        Slot& operator=(const Slot&) = delete;

        const bool isEmpty() const;

        void addItem(Item* item_id);
        Item* takeItem();

        const Id getItemId() const;
        const unsigned int getAmount() const; 
};

#endif