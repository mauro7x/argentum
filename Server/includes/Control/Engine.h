#ifndef __ENGINE_H__
#define __ENGINE_H__

//-----------------------------------------------------------------------------
#include <stdio.h>

#include <atomic>
#include <chrono>
#include <cmath>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Thread.h"
#include "../../../Common/includes/types.h"
#include "../paths.h"
#include "../Model/config_structs.h"
#include "../Model/Game.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ActiveClients.h"
#include "Command.h"
#include "Database.h"
#include "NewConnection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Engine : public Thread {
   private:
    std::atomic_bool keep_executing;
    Database& database;

    // Configuración interna
    int rate;

    // Colas a vaciar en cada iteración
    Queue<NewConnection*>& new_connections;  /* conexiones a agregar */
    Queue<InstanceId*> finished_connections; /* conexiones que finalizaron */
    Queue<Command*> commands;                /* comandos a procesar */
    ActiveClients active_clients; /* contenedor de clientes activos */
    Game game;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* PROXY PARA LA CLASE GAME Y SU ADDPLAYER (SOLO PARA PROBAR) */
    InstanceId _GameAddPlayer(const CharacterCfg& init_data);

    /* Inicializa recursos internos */
    void _init();

    /* Procesa las solicitudes de nuevas conexiones */
    void _processNewConnections();

    /* Elimina las conexiones que finalizaron del contenedor de clientes */
    void _processFinishedConnections();

    /* Vacía las colas sin procesarlas para salir ordenadamente */
    void _freeQueues();

    /* Iteración del loop */
    void _loopIteration(int it);

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Engine(Database& database, Queue<NewConnection*>& new_connections);

    /* Deshabilitamos el constructor por copia. */
    Engine(const Engine&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Engine& operator=(const Engine&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Engine(Engine&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Engine& operator=(Engine&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicia el ciclo del servidor */
    void run() override;

    /* Detiene el ciclo principal */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Engine();
};

//-----------------------------------------------------------------------------

#endif  // __ENGINE_H__
