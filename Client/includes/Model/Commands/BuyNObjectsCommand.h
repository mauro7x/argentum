#ifndef __BUY_N_OBJECTS_COMMAND_H__
#define __BUY_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class BuyNObjectsCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    BuyNObjectsCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~BuyNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __BUY_N_OBJECTS_COMMAND_H__
