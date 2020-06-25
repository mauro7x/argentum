#ifndef __LIST_CONNECTED_PLAYERS_COMMAND_H__
#define __LIST_CONNECTED_PLAYERS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ListConnectedPlayersCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    ListConnectedPlayersCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ListConnectedPlayersCommand();
};

//-----------------------------------------------------------------------------

#endif  // __LIST_CONNECTED_PLAYERS_COMMAND_H__
