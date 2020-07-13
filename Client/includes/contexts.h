#ifndef __CONTEXTS_H__
#define __CONTEXTS_H__

//-----------------------------------------------------------------------------

/** Contextos de ejecución.
 * Nuestro cliente alternará entre estos contextos según el usuario lo solicite,
 * hasta que decida salir del juego.
 */

enum Context {
    HOME_CTX,
    TUTORIAL_CTX,
    CONNECTION_CTX,
    SIGNUP_CTX,
    GAME_CTX,
    EXIT_CTX
};

//-----------------------------------------------------------------------------

#endif  // __CONTEXTS_H__