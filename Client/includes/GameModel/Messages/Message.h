#ifndef __MESSAGE_H__
#define __MESSAGE_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Exceptions/Exception.h"
#include "../../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../colors.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../GameView/Character.h"
#include "../../GameView/CharacterContainer.h"
#include "../../GameView/HUD.h"
#include "../../GameView/Player.h"
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

    /* Agrega el mensaje a la consola de manera polimórfica. En caso de ser un
     * mensaje general, lo agrega a la cabeza de los personajes */
    virtual void update(HUD& hud, Player& player,
                        CharacterContainer& characters) const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Message() {}
};

//-----------------------------------------------------------------------------

#endif  // __MESSAGE_H__
