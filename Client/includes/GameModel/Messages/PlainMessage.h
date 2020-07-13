#ifndef __PLAIN_MESSAGE_H__
#define __PLAIN_MESSAGE_H__

//-----------------------------------------------------------------------------
#include "Message.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class PlainMessage : public Message {
   private:
    std::string content;
    Color color;

   public:
    /* Constructor */
    PlainMessage(const std::string& content, const Color& color);

    //-------------------------------------------------------------------------

    void update(
        HUD& hud, Player& player,
        CharacterContainer& characters) const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~PlainMessage();
};

//-----------------------------------------------------------------------------

#endif  // __PLAIN_MESSAGE_H__
