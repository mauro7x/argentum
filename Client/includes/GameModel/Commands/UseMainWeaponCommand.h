#ifndef __USE_MAIN_WEAPON_COMMAND_H__
#define __USE_MAIN_WEAPON_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/types.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UseMainWeaponCommand : public Command {
   private:
    InstanceId target;

   public:
    /* Constructor */
    UseMainWeaponCommand(const InstanceId& target);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UseMainWeaponCommand();
};

//-----------------------------------------------------------------------------

#endif  // __USE_MAIN_WEAPON_COMMAND_H__
