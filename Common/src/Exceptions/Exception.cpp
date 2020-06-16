#include "../../includes/Exceptions/Exception.h"

//-----------------------------------------------------------------------------
// API Pública

Exception::Exception(const char* fmt, ...) noexcept {
    // int _errno = errno;
    va_list args;
    va_start(args, fmt);
    // int s = vsnprintf(msg_error, BUF_LEN, fmt, args);
    vsnprintf(msg_error, BUF_LEN, fmt, args);
    va_end(args);

    // const char errno_format[] = "\nERRNO_INFO: ";
    // int f = strlen(errno_format);

    // strncpy(msg_error + s, errno_format, BUF_LEN - s);
    // strncpy(msg_error + s + f, strerror(_errno), BUF_LEN - s - f);

    msg_error[BUF_LEN - 1] = 0;
}

const char* Exception::what() const noexcept {
    return msg_error;
}

Exception::~Exception() noexcept {}

//-----------------------------------------------------------------------------
