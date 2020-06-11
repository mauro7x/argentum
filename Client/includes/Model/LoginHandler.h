#ifndef __LOGINHANDLER_H__
#define __LOGINHANDLER_H__

//-----------------------------------------------------------------------------
#include <string>
#include <exception>
//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------



class InvalidUsernameException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};

class InvalidPasswordException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};



class LoginHandler{
    // atributos del loginhandler
   private:

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname, portname.
     */
    LoginHandler();

    /* Deshabilitamos el constructor por copia. */
    LoginHandler(const LoginHandler&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    LoginHandler& operator=(const LoginHandler&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    LoginHandler(LoginHandler&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    LoginHandler& operator=(LoginHandler&& other) = delete;

    /**
     * Descripción: poner a correr al clientareceiver
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void creatVerification(std::string& username, std::string& password);


    /**
     * Descripción: destructor.
     */
    ~LoginHandler();
};

#endif //LOGINHANDLER
