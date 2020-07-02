#ifndef __LIST_H
#define __LIST_H

//-----------------------------------------------------------------------------
#include <list>
#include <string>
//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/json.hpp"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

using json = nlohmann::json;

/*
 * Notificación de list. Lista elementos que tiene disponibles algún NPC. Consta
 * de un mensaje base intruductorio y una lista de strings.
 */
class List : public Notification {
   private:
    json j;

   public:
    /* Constructor */
    List(const std::string& initial_msg,
         const std::list<std::string>& list_items);

    /* Habilitamos el constructor por copia. */
    List(const List& other);

    /* Habilitamos el operador= para copia.*/
    List& operator=(const List& other);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~List();
};

#endif  //__LIST_H