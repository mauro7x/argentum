#ifndef __HEAL_COMMAND_H__
#define __HEAL_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HealCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    HealCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~HealCommand();
};

//-----------------------------------------------------------------------------

#endif  // __HEAL_COMMAND_H__
