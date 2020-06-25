#include "../../includes/Control/Commands/Command.h"

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Protocol.h"
#include "../../includes/Control/Commands/BuyItemCommand.h"
#include "../../includes/Control/Commands/DepositGoldOnBankCommand.h"
#include "../../includes/Control/Commands/DepositItemOnBankCommand.h"
#include "../../includes/Control/Commands/DropCommand.h"
#include "../../includes/Control/Commands/EquipCommand.h"
#include "../../includes/Control/Commands/ListCommand.h"
#include "../../includes/Control/Commands/MeditateCommand.h"
#include "../../includes/Control/Commands/SelfResurrectCommand.h"
#include "../../includes/Control/Commands/SellItemCommand.h"
#include "../../includes/Control/Commands/StartMovingDownCommand.h"
#include "../../includes/Control/Commands/StartMovingLeftCommand.h"
#include "../../includes/Control/Commands/StartMovingRightCommand.h"
#include "../../includes/Control/Commands/StartMovingUpCommand.h"
#include "../../includes/Control/Commands/StopMovingCommand.h"
#include "../../includes/Control/Commands/TakeCommand.h"
#include "../../includes/Control/Commands/UseWeaponCommand.h"
#include "../../includes/Control/Commands/WithdrawGoldFromBankCommand.h"

Command::Command() {}
Command::~Command() {}

// VERIFICAR SI SOCKET RECIBE BIEN LAS COSAS!
// VERIFICAR SI SOCKET RECIBE BIEN LAS COSAS!
// VERIFICAR SI SOCKET RECIBE BIEN LAS COSAS!
Command* CommandFactory::newCommand(InstanceId caller, uint8_t opcode,
                                    SocketWrapper& socket) {
    switch (opcode) {
        case START_MOVING_UP_CMD: {
            return new StartMovingUpCommand(caller);
        }

        case START_MOVING_DOWN_CMD: {
            return new StartMovingDownCommand(caller);
        }

        case START_MOVING_RIGHT_CMD: {
            return new StartMovingRightCommand(caller);
        }

        case START_MOVING_LEFT_CMD: {
            return new StartMovingLeftCommand(caller);
        }

        case STOP_MOVING_CMD: {
            return new StopMovingCommand(caller);
        }

        case MEDITATE_CMD: {
            return new MeditateCommand(caller);
        }

        case SELF_RESURRECT_CMD: {
            return new SelfResurrectCommand(caller);
        }

        case GRAB_OBJECT_CMD: {
            return new TakeCommand(caller);
        }

        case USE_MAIN_WEAPON_CMD: {
            uint32_t target_id;
            socket >> target_id;
            return new UseWeaponCommand(caller, target_id);
        }

        case BUY_OBJECT_CMD: {
            uint32_t x_coord, y_coord;
            Id item_id;
            socket >> x_coord;
            socket >> y_coord;
            socket >> item_id;
            return new BuyItemCommand(caller, x_coord, y_coord, item_id, 1);
        }

        case BUY_N_OBJECTS_CMD: {
            uint32_t x_coord, y_coord, amount;
            Id item_id;
            socket >> x_coord;
            socket >> y_coord;
            socket >> item_id;
            socket >> amount;
            return new BuyItemCommand(caller, x_coord, y_coord, item_id,
                                      amount);
        }

        case SELL_OBJECT_CMD: {
            uint32_t x_coord, y_coord;
            uint8_t n_slot;
            socket >> x_coord;
            socket >> y_coord;
            socket >> n_slot;
            return new SellItemCommand(caller, x_coord, y_coord, n_slot, 1);
        }

        case SELL_N_OBJECTS_CMD: {
            uint32_t x_coord, y_coord, amount;
            uint8_t n_slot;
            socket >> x_coord;
            socket >> y_coord;
            socket >> n_slot;
            socket >> amount;
            return new SellItemCommand(caller, x_coord, y_coord, n_slot,
                                       amount);
        }

        case DEPOSIT_GOLD_CMD: {
            uint32_t x_coord, y_coord, amount;
            socket >> x_coord;
            socket >> y_coord;
            socket >> amount;
            return new DepositGoldOnBankCommand(caller, x_coord, y_coord,
                                                amount);
        }

        case WITHDRAW_GOLD_CMD: {
            uint32_t x_coord, y_coord, amount;
            socket >> x_coord;
            socket >> y_coord;
            socket >> amount;
            return new WithdrawGoldFromBankCommand(caller, x_coord, y_coord,
                                                   amount);
        }

        case DEPOSIT_OBJECT_CMD: {
            uint32_t x_coord, y_coord;
            uint8_t n_slot;
            socket >> x_coord;
            socket >> y_coord;
            socket >> n_slot;
            return new DepositItemOnBankCommand(caller, x_coord, y_coord,
                                                n_slot, 1);
        }

        case DEPOSIT_N_OBJECTS_CMD: {
            uint32_t x_coord, y_coord, amount;
            uint8_t n_slot;
            socket >> x_coord;
            socket >> y_coord;
            socket >> n_slot;
            socket >> amount;
            return new DepositItemOnBankCommand(caller, x_coord, y_coord,
                                                n_slot, amount);
        }

        case THROW_OBJECT_CMD: {
            uint8_t n_slot;
            socket >> n_slot;
            return new DropCommand(caller, n_slot, 1);
        }

        case THROW_N_OBJECTS_CMD: {
            uint8_t n_slot;
            uint32_t amount;
            socket >> n_slot;
            socket >> amount;
            return new DropCommand(caller, n_slot, amount);
        }

        case EQUIP_OBJECT_CMD: {
            uint8_t n_slot;
            socket >> n_slot;
            return new EquipCommand(caller, n_slot);
        }

        case LIST_CMD: {
            uint32_t x_coord, y_coord;
            socket >> x_coord;
            socket >> y_coord;
            return new ListCommand(caller, x_coord, y_coord);
        }

        default: {
            throw Exception("Unknown command received.\n");
        }
    }
}
