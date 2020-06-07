#ifndef __CLOSED_SOCKET_EXCEPTION_H__
#define __CLOSED_SOCKET_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <utility>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClosedSocketException : public Exception {
   public:
    template <typename... Args>
    explicit ClosedSocketException(const char* fmt, Args&&... args) noexcept
        : Exception(fmt, std::forward<Args>(args)...) {}
};

//-----------------------------------------------------------------------------
#endif  // __CLOSED_SOCKET_EXCEPTION_H__
