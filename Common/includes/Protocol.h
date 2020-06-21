#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

//-----------------------------------------------------------------------------

enum Opcode {
    // Server -> Client opcodes
    REPLY_OPCODE = 0,
    PRIVATE_MESSAGE_OPCODE,
    BROADCAST_OPCODE,

    // Client -> Server opcodes
    COMMAND_OPCODE = 128,
    SIGN_IN_OPCODE,
    SIGN_UP_OPCODE
};

enum CommandOpcode {
    // Client -> Server command opcodes

    // Movimiento
    START_MOVING_UP_CMD = 0,
    START_MOVING_DOWN_CMD,
    START_MOVING_LEFT_CMD,
    START_MOVING_RIGHT_CMD,
    STOP_MOVING_CMD,

    // Ataque/hechizos
    USE_MAIN_WEAPON_CMD,

    // Objetos
    EQUIP_OBJECT_CMD,
    GRAB_OBJECT_CMD,
    THROW_OBJECT_CMD,
    THROW_N_OBJECTS_CMD,

    // Generales
    MEDITATE_CMD,
    SELF_RESURRECT_CMD,

    // Interacción con NCPS
    RESURRECT_CMD,          /* Sacerdote */
    HEAL_CMD,               /* Sacerdote */
    LIST_CMD,               /* Banquero y Comerciante */
    DEPOSIT_OBJECT_CMD,     /* Banquero */
    DEPOSIT_N_OBJECTS_CMD,  /* Banquero */
    DEPOSIT_GOLD_CMD,       /* Banquero */
    WITHDRAW_OBJECT_CMD,    /* Banquero */
    WITHDRAW_N_OBJECTS_CMD, /* Banquero */
    WITHDRAW_GOLD_CMD,      /* Banquero */
    BUY_OBJECT_CMD,         /* Comerciante */
    BUY_N_OBJECTS_CMD,      /* Comerciante */
    SELL_OBJECT_CMD,        /* Comerciante */
    SELL_N_OBJECTS_CMD,     /* Comerciante */

    // Comunicación con otros jugadores
    LIST_CONNECTED_PLAYERS_CMD,
    SEND_PRIVATE_MESSAGE_CMD

};

enum ReplyOpcode {
    ERROR_REPLY = 0,
    INFO_REPLY,    /* Blanco */
    SUCCESS_REPLY, /* Verde */
    LIST_REPLY     /* Amarillo */

};

enum BroadcastType {
    NEW, 
    UPDATE, 
    DELETE};

enum EntityType {
    CHARACTER, 
    CREATURE,
    ITEM};

//-----------------------------------------------------------------------------

#endif  // __PROTOCOL_H__