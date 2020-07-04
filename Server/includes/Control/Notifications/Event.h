#ifndef __EVENT_H
#define __EVENT_H

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------
/*
 * Un evento sirve para notificar una acción que afecte la representación
 * multimedia en el cliente.
 */
class Event : public Notification {
   private:
    uint32_t x_coord;
    uint32_t y_coord;
    EventType event_type;

   public:
    /* Constructor */
    Event(const uint32_t x_coord, const uint32_t y_coord,
          const EventType event_type);

    /* Habilitamos el constructor por copia. */
    Event(const Event& other);

    /* Habilitamos el operador= para copia.*/
    Event& operator=(const Event& other);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Event();
};

#endif  //__EVENT_H
