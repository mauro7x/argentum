#ifndef __LIST_COMMAND_H__
#define __LIST_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ListCommand : public Command {
   private:
    uint32_t x;
    uint32_t y;

   public:
    /* Constructor */
    ListCommand(uint32_t x, uint32_t y);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ListCommand();
};

//-----------------------------------------------------------------------------

#endif  // __LIST_COMMAND_H__
