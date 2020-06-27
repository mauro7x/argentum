#ifndef __SIGNED_MESSAGE_H__
#define __SIGNED_MESSAGE_H__

//-----------------------------------------------------------------------------
#include "Message.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum SignedMessageType { GENERAL_SIGNED_MSG, PRIVATE_SIGNED_MSG };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SignedMessage : public Message {
   private:
    std::string sender;
    std::string content;
    SignedMessageType type;
    Color color;

   public:
    /* Constructor */
    SignedMessage(const std::string& sender, const std::string& content,
                  SignedMessageType type, const Color& color);

    //-------------------------------------------------------------------------

    void update(HUD& hud) const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SignedMessage();
};

//-----------------------------------------------------------------------------

#endif  // __SIGNED_MESSAGE_H__
