#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <stdio.h>

#include <atomic>
#include <chrono>

#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Thread.h"
#include "../../../Common/includes/UnitData.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define MAP_W_TILES 40
#define MAP_H_TILES 40
#define TILE_W 32
#define TILE_H 32

#define SPEED 1 /* tiles x sec */
#define TPS 20  /* ticks x sec */

#define TIME_TO_MOVE_A_TILE (1000 / SPEED) /* timepo en ms */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ServerProxy : public Thread {
   private:
    Queue<int*>& requests;
    Queue<PlayerData*>& broadcast;
    PlayerData player;
    std::atomic_bool is_running;
    std::atomic_bool keep_running;

   public:
    /**
     * Descripción: constuctor.
     *
     * Parámetros: -
     */
    ServerProxy(Queue<int*>& requests, Queue<PlayerData*>& broadcast);

    /**
     * Descripción: código que se ejecutará al correr el hilo.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    virtual void run() override;

    /**
     * Descripción: chequea si el hilo se está ejecutando.
     *
     * Parámetros: -
     *
     * Retorno: TRUE si el hilo se está ejecutando, FALSE si no.
     */
    bool isRunning();

    void kill();

    /**
     * Descripción: destructor.
     */
    virtual ~ServerProxy();
};

//-----------------------------------------------------------------------------
#endif  // __SERVER_PROXY_H__
