#ifndef __RESPONSE_H__
#define __RESPONSE_H__
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Protocol.h"
//-----------------------------------------------------------------------------

struct Response {
    Response(const bool succeeded, const std::string& msg,
             const MessageType message_type);
    ~Response();

    // Inhabilito copia
    Response(const Response&) = delete;
    Response& operator=(const Response&) = delete;

    // Habilito movimiento
    Response(Response&& other);
    Response& operator=(Response&& other);

    bool succeeded;
    std::string msg;
    MessageType message_type;
};

//-----------------------------------------------------------------------------
#endif  // __RESPONSE_H__
//-----------------------------------------------------------------------------
