#ifndef __ITEM_BROADCAST_H
#define __ITEM_BROADCAST_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/DataStructs.h"
#include "../../../../Common/includes/json.hpp"
#include "../../../../Common/includes/json_conversion.h"
#include "../../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

using json = nlohmann::json;

/* Notificación de Broadcast en carga de enviar el broadcast del juego
al cliente*/

class ItemBroadcast : public Notification {
   private:
    Id map;
    uint32_t x_coord, y_coord;
    BroadcastType broadcast_type;
    EntityType entity_type;
    json j;

   public:
    /* Constructor para data de items */
    ItemBroadcast(ItemData& data, Id map, BroadcastType broadcast_type);

    /* Habilitamos el constructor por copia. */
    ItemBroadcast(const ItemBroadcast& other);

    /* Habilitamos el operador= para copia.*/
    ItemBroadcast& operator=(const ItemBroadcast& other);
    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    virtual const Id getMapId() const;

    virtual const bool isBroadcast() const;

    virtual const EntityType getEntityType() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~ItemBroadcast();
};

#endif  //__ITEM_BROADCAST_H