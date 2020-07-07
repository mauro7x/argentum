#ifndef __CLOSED_QUEUE_EXCEPTION_H__
#define __CLOSED_QUEUE_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <string>

#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClosedQueueException : public Exception {
   public:
    explicit ClosedQueueException(const std::string msg_error) noexcept
        : Exception(msg_error) {}
};

//-----------------------------------------------------------------------------
#endif  // __CLOSED_QUEUE_EXCEPTION_H__
