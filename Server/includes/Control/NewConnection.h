#ifndef __NEW_CONNECTION_H__
#define __NEW_CONNECTION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../Model/config_structs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct NewConnection {
    SocketWrapper peer;
    CharacterCfg data;

    NewConnection(SocketWrapper& peer, CharacterCfg& data)
        : peer(std::move(peer)), data(data) {}
};

//-----------------------------------------------------------------------------

#endif  // __NEW_CONNECTION_H__
