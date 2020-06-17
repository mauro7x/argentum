#include "../../includes/Control/Engine.h"

//-----------------------------------------------------------------------------
// Métodos privados

/* PROXY QUE VA A SER Game.addPlayer() */
InstanceId next_id_hardcoded = 100;
InstanceId Engine::_GameAddPlayer(const PlayerData& init_data) {
    return next_id_hardcoded++; /* hardcodeamos una id */
}

void Engine::_init() {
    // Cargamos la configuración
    json config = JSON::loadJsonFile(ENGINE_CONFIG_FILEPATH);
    int ticks_per_second = config["ticks_per_second"];
    rate = 1000 / ticks_per_second; /* ms por cada tick (floor) */
}

void Engine::_processFinishedConnections() {
    InstanceId* finished_connection = NULL;
    while ((finished_connection = finished_connections.pop())) {
        fprintf(stderr, "ENGINE: Eliminando una conexión terminada...\n");

        active_clients.remove(*finished_connection);
        // acá habría que persistir al cliente que se desconecto

        delete finished_connection;
    }
}

void Engine::_processNewConnections() {
    NewConnection* new_connection = NULL;
    while ((new_connection = new_connections.pop())) {
        fprintf(stderr, "ENGINE: Procesando una nueva conexión...\n");

        InstanceId id = _GameAddPlayer((*new_connection).data);
        active_clients.add(id, (*new_connection).peer);

        delete new_connection;
    }
}

void Engine::_freeQueues() {
    {
        InstanceId* p = NULL;
        while ((p = finished_connections.pop())) {
            delete p;
        }
    }

    {
        NewConnection* p = NULL;
        while ((p = new_connections.pop())) {
            delete p;
        }
    }

    {
        Command* p = NULL;
        while ((p = commands.pop())) {
            delete p;
        }
    }
}

void Engine::_loopIteration(int it) {
    _processFinishedConnections();
    _processNewConnections();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Engine::Engine(Database& database,
               NonBlockingQueue<NewConnection*>& new_connections)
    : keep_executing(true),
      database(database),
      rate(0),
      new_connections(new_connections),
      finished_connections(),
      commands(),
      active_clients(commands, finished_connections) {}

void Engine::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del engine.\n");

    // Inicializamos recursos necesarios
    _init();

    // Variables para controlar el frame-rate
    auto t1 = std::chrono::steady_clock::now();
    auto t2 = t1;
    std::chrono::duration<float, std::milli> diff;
    int rest = 0, behind = 0, lost = 0;
    int it = 1;

    // Loop principal
    while (keep_executing) {
        _loopIteration(it);

        // Controlamos el rate y verificamos pérdida de frames.
        // Idea de implementación:
        // https://eldipa.github.io/book-of-gehn/articles/2019/10/23/Constant-Rate-Loop.html
        it = 0;
        t2 = std::chrono::steady_clock::now();
        diff = t2 - t1;
        rest = rate - std::ceil(diff.count());

        if (rest < 0) {
            fprintf(stderr, "\n\n=== PÉRDIDA DE FRAME/S ===\n\n\n");
            behind = -rest;
            lost = rate + (behind - behind % rate);
            rest = rate - behind % rate;
            t1 += std::chrono::milliseconds(lost);
            it += std::floor(lost / rate);
        }

        // fprintf(stderr, "ENGINE: Sleeping for %i ms.\n", rest);
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1 += std::chrono::milliseconds(rate);
        it += 1;
    }

    //-------------------------------------------------------------------------
    // Salimos ordenadamente:

    // Terminamos las conexiones forzosamente
    active_clients.stop();

    // Vaciamos las colas para no perder memoria:
    _freeQueues();

    // Persistir

    fprintf(stderr, "DEBUG: Termina la ejecución del engine.\n");
}

void Engine::stop() {
    keep_executing = false;
}

Engine::~Engine() {}

//-----------------------------------------------------------------------------
