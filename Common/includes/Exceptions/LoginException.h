#ifndef __LOGIN_EXCEPTION_H__
#define __LOGIN_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <utility>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class LoginException : public Exception {
   public:
    template <typename... Args>
    explicit LoginException(const char* fmt, Args&&... args) noexcept
        : Exception(fmt, std::forward<Args>(args)...) {}
};

//-----------------------------------------------------------------------------
#endif  // __LOGIN_EXCEPTION_H__
