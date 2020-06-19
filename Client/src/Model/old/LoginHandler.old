#include "../../includes/Model/LoginHandler.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------
#define MAX_CARACTERS 16
//-----------------------------------------------------------------------------
// API Pública

const char* InvalidUsernameException::what() const noexcept {
    return "la cantidad de caracter del usuario ha superado 16 caracteres.";
}

const char* InvalidPasswordException::what() const noexcept {
    return "la cantidad de caracteres de la contrasenia ha superado 16 "
           "caracteres.";
}

void LoginHandler::creatVerification(std::string& username,
                                     std::string& password) {
    if (username.size() > MAX_CARACTERS) {
        throw InvalidUsernameException();
    }
    if (password.size() > MAX_CARACTERS) {
        throw InvalidPasswordException();
    }
}
