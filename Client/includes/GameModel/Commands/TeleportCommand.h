#ifndef __TELEPORT_COMMAND_H__
#define __TELEPORT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class TeleportCommand : public Command {
   private:
    uint32_t x;
    uint32_t y;
    uint32_t map_id;

   public:
    /* Constructor */
    TeleportCommand(uint32_t x, uint32_t y, uint32_t map_id);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~TeleportCommand();
};

//-----------------------------------------------------------------------------

#endif  // __TELEPORT_COMMAND_H__
