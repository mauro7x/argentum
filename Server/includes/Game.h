#ifndef __GAME_H__
#define __GAME_H__

/**
 * Esta clase implementa el patron de codigo de comand
 * El metodo execute se usa con override para ejecutar un comando
 */
class Game {
private:
    // Atributos del Comando
    int player_id;

public:
    /**
     * Descripción: constructor.
     *
     * 
     */
    Game();

    /* Deshabilitamos el constructor por copia. */
    Game(const Game&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Game& operator=(const Game&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Game(Game&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Game& operator=(Game&& other) = delete;

    /**
     * Descripción: destructor.
     */
    ~Game();
};

#endif  // __GAME_H__
