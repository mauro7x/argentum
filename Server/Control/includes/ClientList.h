#ifndef __ClientList_H__
#define __ClientList_H__

#include <unordered_map>
#include <mutex>

#include "../includes/ClientConnection.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"

class ClientList {
private:
    // atributos del ClientConnection
    std::unordered_map<int, ClientConnection> connection_map;
    std::mutex m;

public:
    /**
     * Descripci�n: constructor.
     *
     * Par�metros: id del player que usa la connexion
     */
    ClientList();

    /* Deshabilitamos el constructor por copia. */
    ClientList(const ClientList&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientList& operator=(const ClientList&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientList(ClientList&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientList& operator=(ClientList&& other) = delete;

    bool addPlayer(int player_id, SocketWrapper&& sock);
    /**
     * Descripci�n: destructor.
     */
    ~ClientList();
};

#endif  // __ClientList_H__
