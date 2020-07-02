#ifndef __ENTITY_BROADCAST_H
#define __ENTITY_BROADCAST_H

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

class EntityBroadcast : public Notification {
   private:
    InstanceId id;
    Id map;
    BroadcastType broadcast_type;
    EntityType entity_type;
    json j;

   public:
    /* Constructor para data de characters */
    EntityBroadcast(InstanceId id, PlayerData& data,
                          BroadcastType broadcast_type);

    /* Constructor para data de creatures */
    EntityBroadcast(InstanceId id, CreatureData& data,
                          BroadcastType broadcast_type);

    /* Habilitamos el constructor por copia. */
    EntityBroadcast(const EntityBroadcast& other);

    /* Habilitamos el operador= para copia.*/
    EntityBroadcast& operator=(const EntityBroadcast& other);
    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    virtual const Id getMapId() const;

    virtual const bool isBroadcast() const;

    virtual const EntityType getEntityType() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~EntityBroadcast();
};

#endif  //__ENTITY_BROADCAST_H