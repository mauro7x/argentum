#ifndef __MEDITATE_COMMAND_H__
#define __MEDITATE_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MeditateCommand : public Command {
   public:
    /* Constructor */
    MeditateCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MeditateCommand();
};

//-----------------------------------------------------------------------------

#endif  // __MEDITATE_COMMAND_H__
