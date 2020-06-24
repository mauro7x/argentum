#ifndef __COMMAND_H__
#define __COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/BlockingQueue.h"
#include "../../../../Common/includes/Protocol.h"
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
    InstanceId caller;

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

    /* Ejecución polimórfica del comando */
    virtual void exec(Game& game) = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Command() {}
};

// comando para los movimientos
class CommandMovement : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

   public:
    CommandMovement(InstanceId caller, uint8_t cmd);
    ~CommandMovement() {}

    void exec(Game& game) override;
};

// comando de usar armas
class CommandUseWeapon : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    CommandUseWeapon(InstanceId caller, uint8_t cmd, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~CommandUseWeapon() {}

    void exec(Game& game) override;
};

// comando para equipar las armas
class CommandEquip : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint8_t n_slot;

   public:
    CommandEquip(InstanceId caller, uint8_t cmd, const uint8_t n_slot);
    ~CommandEquip() {}

    void exec(Game& game) override;
};

class CommandMediate : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

   public:
    CommandMediate(InstanceId caller, uint8_t cmd);
    ~CommandMediate() {}

    void exec(Game& game) override;
};

class CommandResurrect : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

   public:
    CommandResurrect(InstanceId caller, uint8_t cmd);
    ~CommandResurrect() {}

    void exec(Game& game) override;
};

class CommandList : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    CommandList(InstanceId caller, uint8_t cmd, const uint32_t x_coord,
                const uint32_t y_coord);
    ~CommandList() {}

    void exec(Game& game) override;
};

class CommandDepositItemOnBank : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint8_t n_slot;
    const uint32_t amount;

   public:
    CommandDepositItemOnBank(InstanceId caller, uint8_t cmd,
                             const uint32_t x_coord, const uint32_t y_coord,
                             const uint8_t n_slot, const uint32_t amount);
    ~CommandDepositItemOnBank() {}

    void exec(Game& game) override;
};

class CommandWithdrawItemFromBank : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t item_id;
    const uint32_t amount;

   public:
    CommandWithdrawItemFromBank(InstanceId caller, uint8_t cmd,
                                const uint32_t x_coord, const uint32_t y_coord,
                                const uint32_t item_id, const uint32_t amount);
    ~CommandWithdrawItemFromBank() {}

    void exec(Game& game) override;
};

class CommandGoldMove : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t amount;

   public:
    CommandGoldMove(InstanceId caller, uint8_t cmd, const uint32_t x_coord,
                    const uint32_t y_coord, const uint32_t amount);
    ~CommandGoldMove() {}

    void exec(Game& game) override;
};

class CommandBuy : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t item_id;
    const uint32_t amount;

   public:
    CommandBuy(InstanceId caller, uint8_t cmd,

               const uint32_t x_coord, const uint32_t y_coord,
               const uint32_t item_id, const uint32_t amount);
    ~CommandBuy() {}

    void exec(Game& game) override;
};

class CommandSell : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint8_t n_slot;
    const uint32_t amount;

   public:
    CommandSell(InstanceId caller, uint8_t cmd,

                const uint32_t x_coord, const uint32_t y_coord,
                const uint8_t n_slot, const uint32_t amount);
    ~CommandSell() {}

    void exec(Game& game) override;
};

class CommandTake : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

   public:
    CommandTake(InstanceId caller, uint8_t cmd);
    ~CommandTake() {}

    void exec(Game& game) override;
};

class CommandDrop : public Command {
   private:
    InstanceId caller;
    uint8_t cmd;

    const uint8_t n_slot;
    const uint32_t amount;

   public:
    CommandDrop(InstanceId caller, uint8_t cmd,

                const uint8_t n_slot, const uint32_t amount);
    ~CommandDrop() {}

    void exec(Game& game) override;
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_H__
