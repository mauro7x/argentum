#ifndef __COMMAND_H__
#define __COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Comando abstracto que se sabe enviar por un socket siguiendo el protocolo.
 * Tener en cuenta que los atributos particulares que necesite cada comando
 * para enviarse deben ser agregados como atributos específicos de las clases
 * concretas que hereden de esta. */

class Command {
   public:
    /* Constructor */
    Command() {}

    /* Deshabilitamos el constructor por copia. */
    Command(const Command&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Command& operator=(const Command&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Command(Command&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Command& operator=(Command&& other) = delete;

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    virtual bool send(const SocketWrapper& socket) = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Command() {}
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_H__
