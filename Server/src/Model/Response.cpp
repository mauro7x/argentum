#include "../../includes/Model/Response.h"
//-----------------------------------------------------------------------------

Response::Response(const bool succeeded, const std::string& msg,
                   const MessageType message_type)
    : succeeded(succeeded), msg(msg), message_type(message_type) {}

Response::~Response() {}

Response::Response(Response&& other) {
    this->succeeded = other.succeeded;
    this->msg = other.msg;
    this->message_type = other.message_type;
}

Response& Response::operator=(Response&& other) {
    this->succeeded = other.succeeded;
    this->msg = other.msg;
    this->message_type = other.message_type;
    return *this;
}

//-----------------------------------------------------------------------------
