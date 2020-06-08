#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Game.h"

/**
 * Esta clase implementa el patron de codigo de comand
 * El metodo execute se usa con override para ejecutar un comando
 */
class Command {
private:
    // Atributos del Comando
    int player_id;

public:
    /**
     * Descripci�n: constructor.
     *
     * Par�metros: id del jugador
     */
    Command(int player_id);

    /* Deshabilitamos el constructor por copia. */
    Command(const Command&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Command& operator=(const Command&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Command(Command&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Command& operator=(Command&& other) = delete;


    virtual void execute(Game* game);


    /**
     * Descripci�n: destructor.
     */
    ~Command();
};

#endif  // __COMMAND_H__
