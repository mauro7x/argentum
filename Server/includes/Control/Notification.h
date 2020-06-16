#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Notificación abstracta que se sabe enviar por un socket. Tener en cuenta que
 * los atributos particulares que necesite cada notificación para enviarse deben
 * ser agregados como atributos específicos de las clases concretas que hereden
 * de esta. */

class Notification {
   public:
    /* Constructor */
    Notification() {}

    /* Deshabilitamos el constructor por copia. */
    Notification(const Notification&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Notification& operator=(const Notification&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Notification(Notification&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Notification& operator=(Notification&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const SocketWrapper& peer) = 0;

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Notification() {}
};

//-----------------------------------------------------------------------------

#endif  // __NOTIFICATION_H__
