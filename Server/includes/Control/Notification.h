#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <memory>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "NotificationBroadcast.h"
#include "NotificationMessage.h"
#include "NotificationReply.h"
//-----------------------------------------------------------------------------
/* Notificación abstracta que se sabe enviar por un socket. Tener en cuenta que
 * los atributos particulares que necesite cada notificación para enviarse deben
 * ser agregados como atributos específicos de las clases concretas que hereden
 * de esta. */

class Notification {
   public:
    /* Constructor */
    Notification() {}

    /**
     * Descripción: Generatión de la notificatión depende de los parametros.
     *
     * Parámetros: - booleano is_reply : para checkear si es una respuesta para
     *               comandos
     *             - caracter opcode : para indicar tipo de la respuesta, si no 
     *               es una respuesta, opcode es NULL;
     *             - unit32_t longitud : la cantidad de bytes del mensaje que va 
     *               a enviar al cliente
     *             - string reply : mensaje de formato string a enviar al cliente.
     *             - uint32_t sender_length : la cantidad de bytes del nombre del
     *               sender, 0 si no es un mensaje privado;
     *             - string sender : nombre del sender en formato string, NULL si
     *               no es un mensaje privado;
     *
     * Retorno: clase Notificaction.
     *
     */
    static std::unique_ptr<Notification> getNotification(bool is_reply, char opcode, uint32_t length,
                                 std::string reply, uint32_t sender_length,
                                 std::string sender);

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
