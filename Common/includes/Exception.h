#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

//-----------------------------------------------------------------------------
#include <exception>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Exception : public std::exception {
   private:
    std::string msg_error;

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: mensaje de error.
     */
    explicit Exception(const std::string msg_error) noexcept;

    /**
     * Descripción: devuelve el mensaje de error.
     *
     * Parámetros: -
     *
     * Retorno: mensaje de error.
     *
     * No hay que ocuparse del puntero recibido.
     */
    virtual const char* what() const noexcept;

    /**
     * Descripción: destructor.
     */
    virtual ~Exception() noexcept;
};

//-----------------------------------------------------------------------------
#endif  // __EXCEPTION_H__
