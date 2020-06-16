#ifndef __SLOT_H__
#define __SLOT_H__

#include "../../../Common/includes/types.h"
#include "Item.h"

#define EMPTY_ITEM_ID 0

class Slot {
    private:
        Item* item;
        unsigned int amount;

    public:
        Slot();
        ~Slot();

        Slot(Slot&& other);
        Slot& operator=(Slot&& other);

        Slot(const Slot&) = delete;
        Slot& operator=(const Slot&) = delete;

        const bool isEmpty() const;

        void addItem(Item* item_id);
        Item* takeItem();
};

#endif