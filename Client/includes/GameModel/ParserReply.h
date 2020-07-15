#ifndef __PARSER_REPLY_H__
#define __PARSER_REPLY_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct ParserReply {
    // Flags
    bool log_input;
    bool clear_selection;

    // Contenido
    std::string msg;

    // Constructor por defecto
    ParserReply() : log_input(true), clear_selection(false) {}
};

//-----------------------------------------------------------------------------

#endif  // __PARSER_REPLY_H__
