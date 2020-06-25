#ifndef __SELL_N_OBJECTS_COMMAND_H__
#define __SELL_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SellNObjectsCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    SellNObjectsCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SellNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __SELL_N_OBJECTS_COMMAND_H__
