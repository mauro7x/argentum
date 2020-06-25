#ifndef __BROADCAST_FACTORY_H__
#define __BROADCAST_FACTORY_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/DataStructs.h"
#include "../../../../Common/includes/JSON.h"
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/json_conversion.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "DeleteCharacterBroadcast.h"
#include "DeleteCreatureBroadcast.h"
#include "NewCharacterBroadcast.h"
#include "NewCreatureBroadcast.h"
#include "NewPlayerBroadcast.h"
#include "UpdateCharacterBroadcast.h"
#include "UpdateCreatureBroadcast.h"
#include "UpdatePlayerBroadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class BroadcastFactory {
   public:
    static Broadcast* newBroadcast(uint8_t broadcast_type, uint8_t entity_type,
                                   const SocketWrapper& socket);
};
//-----------------------------------------------------------------------------

#endif  // __BROADCAST_FACTORY_H__
