#ifndef __COMMAND_H__
#define __COMMAND_H__

//-----------------------------------------------------------------------------
#include "../Model/Game.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Comando abstracto que se sabe ejecutar. Puede recibir parámetros en el método
 * de ejecución según sea necesario. Tener en cuenta que los atributos
 * particulares que necesite cada comando deben ser agregados como atributos
 * específicos de las clases concretas que hereden de esta. */

class Command {
    protected:
        InstanceId caller;

    public:
        /* Constructor */
        Command(InstanceId caller);

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
        virtual void operator()(Game& game) = 0;

        //-------------------------------------------------------------------------

        /* Destructor */
        virtual ~Command();
};

class CommandProxy: public Command {
    private:
        char cmd;
    
    public:
        CommandProxy(InstanceId caller, char cmd);
        ~CommandProxy();

        virtual void operator()(Game& game) override;
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_H__
