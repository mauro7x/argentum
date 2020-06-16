#ifndef __NEW_CONNECTION_H__
#define __NEW_CONNECTION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/UnitData.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct NewConnection {
    SocketWrapper peer;
    PlayerData data;

    NewConnection(SocketWrapper& peer, PlayerData& data)
        : peer(std::move(peer)), data(data) {}
};

//-----------------------------------------------------------------------------

#endif  // __NEW_CONNECTION_H__
