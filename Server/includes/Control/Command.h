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

//comando para los movimientos
class CommandMovemente: public Command {
    private:
     SocketWrapper& peer;
    
    public:
        CommandMovemente(InstanceId caller, char cmd, SocketWrapper& peer);
        ~CommandMovemente();

        virtual void operator()(Game& game) override;
};

//comando de usar armas
class CommandUseWeapon: public Command {
    private:
     SocketWrapper& peer;
     const uint32_t x_coord;
     const uint32_t y_coord;
    
    public:
        CommandUseWeapon(InstanceId caller, char cmd, SocketWrapper& peer, const uint32_t x_coord, const uint32_t y_coord);
        ~CommandUseWeapon();

        virtual void operator()(Game& game) override;
};

//comando para equipar las armas
class CommandEquip: public Command {
    private:
     SocketWrapper& peer;
     const uint8_t n_slot;
    
    public:
        CommandEquip(InstanceId caller, char cmd, SocketWrapper& peer, const uint8_t n_slot);
        ~CommandEquip();

        virtual void operator()(Game& game) override;
};


class CommandMediate: public Command {
    private:
     SocketWrapper& peer;
    
    public:
        CommandMediate(InstanceId caller, char cmd, SocketWrapper& peer);
        ~CommandMediate();

        virtual void operator()(Game& game) override;
};

class CommandResurrect: public Command {
    private:
     SocketWrapper& peer;
    
    public:
        CommandResurrect(InstanceId caller, char cmd, SocketWrapper& peer);
        ~CommandResurrect();

        virtual void operator()(Game& game) override;
};

class CommandList: public Command {
    private:
     SocketWrapper& peer;
     const uint32_t x_coord;
     const uint32_t y_coord;
    
    public:
        CommandList(InstanceId caller, char cmd, SocketWrapper& peer, const uint32_t x_coord, const uint32_t y_coord);
        ~CommandList();

        virtual void operator()(Game& game) override;
};

class CommandDepositItemOnBank: public Command {
    private:
     SocketWrapper& peer;
     const uint32_t x_coord;
     const uint32_t y_coord;
    
    public:
        CommandDepositItemOnBank(InstanceId caller, char cmd, SocketWrapper& peer, const uint32_t x_coord, const uint32_t y_coord);
        ~CommandDepositItemOnBank();

        virtual void operator()(Game& game) override;
};



//-----------------------------------------------------------------------------

#endif  // __COMMAND_H__
