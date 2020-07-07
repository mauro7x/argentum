#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <exception>
#include <typeinfo>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define BUF_LEN 256
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Exception : public std::exception {
   private:
    char msg_error[BUF_LEN];

   public:
    /* Constructor */
    explicit Exception(const char* fmt, ...) noexcept;

    /* Mensaje de error */
    virtual const char* what() const noexcept;

    /* Destructor */
    virtual ~Exception() noexcept;
};

//-----------------------------------------------------------------------------
#endif  // __EXCEPTION_H__
