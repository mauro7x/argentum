#ifndef __SDL_EXCEPTION_H__
#define __SDL_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <string>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SDLException : public Exception {
   private:
    const char* sdl_msg_error;

   public:
    explicit SDLException(const std::string msg_error,
                          const char* sdl_msg_error) noexcept
        : Exception(msg_error), sdl_msg_error(sdl_msg_error) {}

    const char* sdlError() const noexcept {
        return sdl_msg_error;
    }
};

//-----------------------------------------------------------------------------
#endif  // __SDL_EXCEPTION_H__
