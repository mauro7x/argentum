#ifndef __NOTIFICATION_BROADCAST_H
#define __NOTIFICATION_BROADCAST_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/json.hpp"
#include "../../../Common/includes/json_conversion.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

using json = nlohmann::json;

/* Notificación de Broadcast en carga de enviar el broadcast del juego
al cliente*/

class NotificationBroadcast : public Notification {
   private:
    InstanceId id;
    Id map;
    BroadcastType broadcast_type;
    EntityType entity_type;
    json j;

   public:
    /* Constructor para data de characters */
    NotificationBroadcast(InstanceId id, PlayerData& data,
                          BroadcastType broadcast_type);

    /* Constructor para data de creatures */
    NotificationBroadcast(InstanceId id, CreatureData& data,
                          BroadcastType broadcast_type);

    /* Constructor para data de items */
    NotificationBroadcast(ItemData& data, BroadcastType broadcast_type);

    /* Constructor para data de item */
    //     NotificationBroadcast(InstanceId id, ItemData& data,
    //                           BroadcastType opcode, EntityType entity_type);
    /* Deshabilitamos el constructor por copia. */
    NotificationBroadcast(const NotificationBroadcast& other);

    /* Deshabilitamos el operador= para copia.*/
    NotificationBroadcast& operator=(const NotificationBroadcast& other);
    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    virtual const Id getMapId() const;

    virtual const bool isBroadcast() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationBroadcast() {}
};

#endif  //__NOTIFICATION_BROADCAST_H