#ifndef __PARTICULAR_EXCEPTION_H__
#define __PARTICULAR_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <utility>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ParticularException : public Exception {
   public:
    template <typename... Args>
    explicit ParticularException(const char* fmt, Args&&... args) noexcept
        : Exception(fmt, std::forward<Args>(args)...) {}
};

//-----------------------------------------------------------------------------
#endif  // __PARTICULAR_EXCEPTION_H__
