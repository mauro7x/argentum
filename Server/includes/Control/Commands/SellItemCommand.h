#ifndef __SELL_ITEM_COMMAND_H__
#define __SELL_ITEM_COMMAND_H__

#include "Command.h"

class SellItemCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint8_t n_slot;
    const uint32_t amount;

   public:
    SellItemCommand(const InstanceId caller, const uint32_t x_coord,
                    const uint32_t y_coord, const uint8_t n_slot,
                    const uint32_t amount);
    ~SellItemCommand();

    void exec(Game& game) override;
};

#endif  // __SELL_ITEM_COMMAND_H__
