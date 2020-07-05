#ifndef __BUY_ITEM_COMMAND_H__
#define __BUY_ITEM_COMMAND_H__

#include "Command.h"

// Compra el objeto que corresponde al id especificado, al vendedor en la
// posición (x_coord, y_coord).
class BuyItemCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t item_id;
    const uint32_t amount;

   public:
    BuyItemCommand(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord, const uint32_t item_id,
                   const uint32_t amount);
    ~BuyItemCommand();

    void exec(Game& game) override;
};

#endif  // __BUY_ITEM_COMMAND_H__
