#ifndef __PARTICULAR_EXCEPTION_H__
#define __PARTICULAR_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <string>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ParticularException : public Exception {
   public:
    explicit ParticularException(const std::string msg_error) noexcept
        : Exception(msg_error) {}
};

//-----------------------------------------------------------------------------
#endif  // __PARTICULAR_EXCEPTION_H__
