#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum Opcode {
    // Server -> Client opcodes
    CONNECTION_ACK_OPCODE = 0,
    MESSAGE_OPCODE,
    BROADCAST_OPCODE,
    EVENT_OPCODE,

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
    UNEQUIP_OBJECT_CMD,
    GRAB_OBJECT_CMD,
    THROW_N_OBJECTS_CMD,

    // Generales
    MEDITATE_CMD,
    SELF_RESURRECT_CMD,

    // Interacción con NCPS
    HELP_NPC_CMD,           /* Cualquier NPC */
    RESURRECT_CMD,          /* Sacerdote */
    HEAL_CMD,               /* Sacerdote */
    LIST_CMD,               /* Banquero y Comerciante */
    DEPOSIT_N_OBJECTS_CMD,  /* Banquero */
    DEPOSIT_GOLD_CMD,       /* Banquero */
    WITHDRAW_N_OBJECTS_CMD, /* Banquero */
    WITHDRAW_GOLD_CMD,      /* Banquero */
    BUY_N_OBJECTS_CMD,      /* Comerciante */
    SELL_N_OBJECTS_CMD,     /* Comerciante */

    // Comunicación con otros jugadores
    LIST_CONNECTED_PLAYERS_CMD,
    SEND_GENERAL_MESSAGE_CMD,
    SEND_PRIVATE_MESSAGE_CMD,

    // Portal
    TRANSPORT_CMD

};

enum ConnectionAckType {
    // Distintos estados
    SUCCESS_ACK = 0,
    ERROR_INVALID_USERNAME_ACK,
    ERROR_INVALID_PASSWORD_ACK,
    ERROR_USERNAME_CONNECTED_ACK,
    ERROR_USERNAME_TAKEN_ACK
};

enum MessageType {
    // Mensajes del servidor
    ERROR_MSG = 0,
    INFO_MSG,
    SUCCESS_MSG,
    LIST_MSG,

    // Mensajes del chat general
    GENERAL_MSG = 128,

    // Mensajes privados
    PRIVATE_MSG,
};

enum BroadcastType { NEW_BROADCAST, UPDATE_BROADCAST, DELETE_BROADCAST };

enum EventType {
    MOVEMENT_EV_TYPE = 0,
    THROW_EV_TYPE,
    GRAB_EV_TYPE,
    MEDITATE_EV_TYPE,
    LEVEL_UP_EV_TYPE,
    DEATH_EV_TYPE,
    RESURRECT_EV_TYPE,
    GRAL_ATTACK_EV_TYPE,
    EXPLOSION_SPELL_EV_TYPE,
    HEALING_SPELL_EV_TYPE,
    HEALED_BY_PRIEST_EV_TYPE
};

enum EntityType { PLAYER_TYPE, CHARACTER_TYPE, CREATURE_TYPE, ITEM_TYPE };

//-----------------------------------------------------------------------------

#endif  // __PROTOCOL_H__
