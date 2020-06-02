#ifndef __CLOSED_QUEUE_EXCEPTION_H__
#define __CLOSED_QUEUE_EXCEPTION_H__

//-----------------------------------------------------------------------------
#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClosedQueueException : public Exception {
   public:
    template <typename... Args>
    explicit ClosedQueueException(const char* fmt, Args&&... args) noexcept
        : Exception(fmt, std::forward<Args>(args)...) {}
};

//-----------------------------------------------------------------------------
#endif  // __CLOSED_QUEUE_EXCEPTION_H__
