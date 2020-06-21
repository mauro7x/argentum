#ifndef __NOTIFICATION_BROADCAST_H
#define __NOTIFICATION_BROADCAST_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/types.h"
#include "../../../Common/includes/json.hpp"
#include "../../../Common/includes/json_conversion.h"
#include "../../../Common/includes/Protocol.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

using json = nlohmann::json;

enum BroadcastType {NEW, UPDATE, DELETE};

/* Notificación de Broadcast en carga de enviar el broadcast del juego 
al cliente*/

class NotificationBroadcast : public Notification {
   private:
        InstanceId id;
        Id map;
        BroadcastType type;
        json j;

   public:
    /* Constructor */
    NotificationBroadcast(InstanceId id, PlayerData& data, BroadcastType opcode);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const SocketWrapper& peer);

    const Id getMapId() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationBroadcast() {}
};

#endif  //__NOTIFICATION_BROADCAST_H