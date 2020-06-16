#include "../../includes/Control/Database.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Database::Database() : initialized(false) {}

void Database::init() {
    std::unique_lock<std::mutex> l(m);
    if (initialized) {
        throw Exception("Database already initialized.");
    }

    // cargar el archivo json
    // por ahora hacemos el proxy:
    {
        clients["mauro"] = "123";
        clients["juan"] = "321";
    }

    initialized = true;
}

void Database::signIn(const std::string& username,
                      const std::string& password) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (clients.count(username) == 0) {
        throw LoginException(
            "El nombre de usuario no coincide con ningún usuario en nuestro "
            "sistema.");
    }

    if (clients.at(username) != password) {
        throw LoginException("La contraseña ingresada es incorrecta.");
    }

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Bienvenido, %s\n", username.c_str());

    /* chequear que el jugador no esté conectado ya (por ahora no podemos)
    throw LoginException("El usuario solicitado se encuentra conectado.");
    */
}

void Database::signUp(const std::string& username,
                      const std::string& password) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (clients.count(username) > 0) {
        throw LoginException(
            "El nombre de usuario solicitado se encuentra en uso.");
    }

    // Crear un nuevo jugador (por ahora, proxy)
    clients[username] = password;

    /* una idea: cuando creamos, al jugador le pedimos que ingrese esto:

    username:
    password:
    clase:
    raza:

    y ahi ya podemos inicializar la data y devolverla.
    */

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Has creado tu personaje. Bienvenido, %s\n",
            username.c_str());
}

Database::~Database() {}

//-----------------------------------------------------------------------------
