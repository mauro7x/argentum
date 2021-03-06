#ifndef __EQUIP_OBJECT_COMMAND_H__
#define __EQUIP_OBJECT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EquipObjectCommand : public Command {
   private:
    int8_t inventory_slot;

   public:
    /* Constructor */
    EquipObjectCommand(int8_t inventory_slot);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~EquipObjectCommand();
};

//-----------------------------------------------------------------------------

#endif  // __EQUIP_OBJECT_COMMAND_H__
