#ifndef __COMMAND_H__
#define __COMMAND_H__
//-----------------------------------------------------------------------------
#include <exception>
//-----------------------------------------------------------------------------
#include "../../../../Common/includes/BlockingQueue.h"
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/types.h"
#include "../../Model/Game.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Comando abstracto que se sabe ejecutar. Puede recibir parámetros en el método
 * de ejecución según sea necesario. Tener en cuenta que los atributos
 * particulares que necesite cada comando deben ser agregados como atributos
 * específicos de las clases concretas que hereden de esta. */
class Command {
   protected:
    const InstanceId caller;

   public:
    /* Constructor */
    Command(const InstanceId caller);

    /* Deshabilitamos el constructor por copia. */
    Command(const Command&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Command& operator=(const Command&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Command(Command&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Command& operator=(Command&& other) = delete;

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del comando */
    virtual void exec(Game& game) = 0;

    /* Devuelve el InstanceId del caller. */
    const InstanceId getCaller() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Command();
};

class CommandFactory {
   public:
    static Command* newCommand(InstanceId caller, uint8_t opcode,
                               SocketWrapper& socket);
};

class UnknownCommandException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_H__
