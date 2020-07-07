#ifndef __SDL_EXCEPTION_H__
#define __SDL_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <utility>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SDLException : public Exception {
   public:
    template <typename... Args>
    explicit SDLException(const char* fmt, Args&&... args) noexcept
        : Exception(fmt, std::forward<Args>(args)...) {}
};

//-----------------------------------------------------------------------------
#endif  // __SDL_EXCEPTION_H__
