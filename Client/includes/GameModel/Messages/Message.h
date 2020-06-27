#ifndef __MESSAGE_H__
#define __MESSAGE_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../colors.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../GameView/HUD.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Message {
   public:
    /* Constructor */
    Message() {}

    /* Deshabilitamos el constructor por copia. */
    Message(const Message&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Message& operator=(const Message&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Message(Message&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Message& operator=(Message&& other) = delete;

    //-------------------------------------------------------------------------

    /* Agrega el mensaje a la consola de manera polimórfica */
    virtual void update(HUD& hud) const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Message() {}
};

//-----------------------------------------------------------------------------

#endif  // __MESSAGE_H__
