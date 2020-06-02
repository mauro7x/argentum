#ifndef __CLOSED_SOCKET_EXCEPTION_H__
#define __CLOSED_SOCKET_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <string>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClosedSocketException : public Exception {
   public:
    explicit ClosedSocketException(const std::string msg_error) noexcept
        : Exception(msg_error) {}
};

//-----------------------------------------------------------------------------
#endif  // __CLOSED_SOCKET_EXCEPTION_H__
