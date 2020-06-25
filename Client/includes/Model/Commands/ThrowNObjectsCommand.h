#ifndef __THROW_N_OBJECTS_COMMAND_H__
#define __THROW_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ThrowNObjectsCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    ThrowNObjectsCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ThrowNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __THROW_N_OBJECTS_COMMAND_H__
