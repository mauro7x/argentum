#ifndef __LOGINVALIDATOR_H__
#define __LOGINVALIDATOR_H__

#include "../../../Common/includes/Socket/SocketWrapper.h"

class LoginValidator {
private:
	// unordered_map<username, <id><password>>
	int player = 1;
public:

	/**
	 * Descripción: Valida que la conexion sea correcta.
	 *
	 * Parámetros: Socket ya conectado con el cliente del cual no se leyo nada
	 *
	 * Retorno: Id del Jugador
	 */
	int loginConnection(SocketWrapper* socket) {
		return player++;
	}
};

#endif  // __LOGINVALIDATOR_H__
