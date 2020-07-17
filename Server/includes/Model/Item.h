#ifndef __ITEM_H__
#define __ITEM_H__
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "Response.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
class Character;  // Evito dependencias circulares debido a double dispatch
//-----------------------------------------------------------------------------

/*
 * Define la abstraccion Item.
 * Un Item es equipable y almacenable
 * en un inventario.
 */
class Item {
   protected:
    const Id id;
    const std::string name;
    const unsigned int price;

   public:
    Item(const Id id, const std::string name, const unsigned int price);
    virtual ~Item();

    virtual Response equip(Character& equipper) = 0;

    virtual const std::string what() const;

    const Id getId() const;

    const unsigned int getPrice() const;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
