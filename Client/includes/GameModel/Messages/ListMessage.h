#ifndef __LIST_MESSAGE_H__
#define __LIST_MESSAGE_H__

//-----------------------------------------------------------------------------
#include "Message.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ListMessage : public Message {
   private:
    std::string content;
    Color color;

   public:
    /* Constructor */
    ListMessage(const std::string& content, const Color& color);

    //-------------------------------------------------------------------------

    void update(HUD& hud) const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ListMessage();
};

//-----------------------------------------------------------------------------

#endif  // __LIST_MESSAGE_H__
