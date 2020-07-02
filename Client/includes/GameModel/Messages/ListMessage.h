#ifndef __LIST_MESSAGE_H__
#define __LIST_MESSAGE_H__

//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Message.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ListMessage : public Message {
   private:
    std::string initial_msg;
    std::list<std::string> list_items;
    Color color;

   public:
    /* Constructor */
    ListMessage(const std::string& initial_msg,
                const std::list<std::string>& list_items, const Color& color);

    //-------------------------------------------------------------------------

    void update(HUD& hud) const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ListMessage();
};

//-----------------------------------------------------------------------------

#endif  // __LIST_MESSAGE_H__
