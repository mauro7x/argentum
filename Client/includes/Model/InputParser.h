#ifndef __INPUT_PARSER_H__
#define __INPUT_PARSER_H__

//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Protocol.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Commands/Command.h"
#include "Commands/Commands.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#define EQUIP_EXPECTED_INPUT "/equipar"
#define GRAB_EXPECTED_INPUT "/tomar"
#define THROW_EXPECTED_INPUT "/tirar"
#define MEDITATE_EXPECTED_INPUT "/meditar"
#define RESURRECT_EXPECTED_INPUT "/resucitar"
#define HEAL_EXPECTED_INPUT "/curar"
#define LIST_EXPECTED_INPUT "/listar"
#define DEPOSIT_EXPECTED_INPUT "/depositar"
#define WITHDRAW_EXPECTED_INPUT "/retirar"
#define BUY_EXPECTED_INPUT "/comprar"
#define SELL_EXPECTED_INPUT "/vender"
#define LIST_PLAYERS_EXPECTED_INPUT "/listar_jugadores"

enum InputCommand {
    EQUIP_INPUT_CMD,
    GRAB_INPUT_CMD,
    THROW_INPUT_CMD,
    MEDITATE_INPUT_CMD,
    RESURRECT_INPUT_CMD,
    HEAL_INPUT_CMD,
    LIST_INPUT_CMD,
    DEPOSIT_INPUT_CMD,
    WITHDRAW_INPUT_CMD,
    BUY_INPUT_CMD,
    SELL_INPUT_CMD,
    LIST_PLAYERS_INPUT_CMD
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class InputParser {
   private:
    std::unordered_map<std::string, InputCommand> commands;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Llena el mapa de comandos según los comandos esperados */
    void _fillCommands();

    /* Parsea un input de tipo COMANDO */
    Command* _parseCommandInput(const std::string& command_input) const;

    /* Parsea COMANDO reconocido */
    Command* _parseCommand(const std::string& command_input,
                           InputCommand cmd_id) const;

    /* Obtiene el identificador del comando */
    std::string _getCommandIdentifier(const std::string& command_input) const;

    /* Obtiene el cuerpo del comando */
    std::string _getCommandBody(const std::string& command_input) const;

    /* Parsea un input de tipo MENSAJE PRIVADO */
    Command* _parseMessageInput(const std::string& message_input) const;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    InputParser();

    /* Deshabilitamos el constructor por copia. */
    InputParser(const InputParser&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    InputParser& operator=(const InputParser&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    InputParser(InputParser&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    InputParser& operator=(InputParser&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Parsea un input y obtiene un comando. Si el input es inválido, retorna
     * NULL. */
    Command* parse(const std::string& input) const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~InputParser();
};

//-----------------------------------------------------------------------------

#endif  // __INPUT_PARSER_H__
