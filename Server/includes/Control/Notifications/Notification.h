#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

//-----------------------------------------------------------------------------
// #include <memory> que hace esto? -m
#include <cstdint>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

typedef std::vector<uint8_t> Serialized;

//-----------------------------------------------------------------------------
/* Notificación abstracta que se sabe enviar por un socket. Tener en cuenta que
 * los atributos particulares que necesite cada notificación para enviarse deben
 * ser agregados como atributos específicos de las clases concretas que hereden
 * de esta. */

class Notification {
   public:
    /* Constructor */
    Notification();

    /* Deshabilitamos el constructor por copia. */
    Notification(const Notification&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Notification& operator=(const Notification&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Notification(Notification&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Notification& operator=(Notification&&) = delete;

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer) = 0;

    virtual const bool isForEveryMap() const;

    virtual const EntityType getEntityType() const;

    virtual const Id getMapId() const;

    virtual const InstanceId getSourceInstanceId() const;

    virtual const bool isEntityBroadcast() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Notification();
};

//-----------------------------------------------------------------------------

#endif  // __NOTIFICATION_H__
