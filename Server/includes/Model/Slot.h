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

    /* Devuelve si el slot está vacío */
    const bool isEmpty() const;

    /* Agrega el item y la cantidad al slot */
    void addItem(Item* item_id, const unsigned int amount);

    /*
     * Obtiene la cantidad especificada del item en el slot.
     *
     * Si la cantidad de items en el slot es menor que amount,
     * se configura en amount la cantidad real droppeada.
     *
     * En caso de no haber item alguno, retorna nullptr.
     */
    Item* takeItem(unsigned int& amount);

    /* Vacía el slot */
    void clearSlot();

    /* Devuelve la id del item que contiene el slot */
    const Id getItemId() const;

    /* Devuelve la cantidad del item que hay en el slot */
    const unsigned int getAmount() const;
};

#endif