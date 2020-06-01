#include "ProtectedSocket.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ProtectedSocket::_setServerAddress(const std::string& port,
                                        addrinfo** address) {
    addrinfo hints;

    // Filtros que nos interesan:
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Servidor
    hints.ai_protocol = 0;

    // Inicializamos en 0 el resto de los valores:
    hints.ai_addr = NULL;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    if (::getaddrinfo(0, &port[0], &hints, address)) {
        throw Exception(SET_SERVER_ADDRESS_ERROR);
    }
}

void ProtectedSocket::_setClientAddresses(const std::string& hostname,
                                          const std::string& port,
                                          addrinfo** addresses) {
    addrinfo hints;

    // Filtros que nos interesan:
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = 0;               // Cliente
    hints.ai_protocol = 0;

    // Inicializamos en 0 el resto de los valores:
    hints.ai_addr = NULL;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;

    if (::getaddrinfo(&hostname[0], &port[0], &hints, addresses)) {
        throw Exception(SET_CLIENT_ADDRESSES_ERROR);
    }
}

void ProtectedSocket::_setFd(addrinfo* address, addrinfo* all_addresses) {
    fd = ::socket(address->ai_family, address->ai_socktype,
                  address->ai_protocol);

    if (fd == -1) {
        ::freeaddrinfo(all_addresses);
        throw Exception(SET_FD_ERROR);
    } else {
        fd_valid = true;
    }
}

void ProtectedSocket::_fixTimeWait(addrinfo* address) {
    int val = 1;
    if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) {
        ::freeaddrinfo(address);
        throw Exception(FIX_TIMEWAIT_ERROR);
    }
}

void ProtectedSocket::_bind(addrinfo* address) {
    int s;
    s = bind(fd, address->ai_addr, address->ai_addrlen);
    ::freeaddrinfo(address);

    if (s == -1) {
        throw Exception(BIND_ERROR);
    }
}

void ProtectedSocket::_listen(const int max_queued_clients) {
    if (listen(fd, max_queued_clients)) {
        throw Exception(LISTEN_ERROR);
    }
}

void ProtectedSocket::_tryToConnectTo(addrinfo* addresses) {
    bool connected = false;
    addrinfo* ptr;
    try {
        for (ptr = addresses; ((ptr != NULL) && (connected == false));
             ptr = ptr->ai_next) {
            _setFd(ptr, addresses);
            if (::connect(fd, ptr->ai_addr, ptr->ai_addrlen)) {
                _closeFdIfValid();
            } else {
                connected = true;
            }
        }

        ::freeaddrinfo(addresses);
        if (!connected) {
            throw Exception(CLIENT_OUT_OF_ADDRESSES);
        }
    } catch (const Exception& e) {
        throw e;
    }
}

void ProtectedSocket::_closeFdIfValid() {
    if (fd_valid) {
        fd_valid = false;
        if (::close(fd)) {
            throw Exception(CLOSE_ERROR);
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ProtectedSocket::ProtectedSocket(const std::string& port,
                                 const int max_queued_clients) {
    std::unique_lock<std::mutex> l(m);
    fd_valid = false;
    addrinfo* address;
    try {
        _setServerAddress(port, &address);
        _setFd(address, address);
        _fixTimeWait(address);
        _bind(address);
        _listen(max_queued_clients);
    } catch (const Exception& e) {
        throw e;
    }
}

ProtectedSocket::ProtectedSocket(const int fd) : fd(fd), fd_valid(true) {}

ProtectedSocket::ProtectedSocket(const std::string& hostname,
                                 const std::string& port) {
    std::unique_lock<std::mutex> l(m);
    addrinfo* addresses;
    try {
        _setClientAddresses(hostname, port, &addresses);
        _tryToConnectTo(addresses);
    } catch (const Exception& e) {
        throw e;
    }
}

int ProtectedSocket::accept() {
    std::unique_lock<std::mutex> l(m);

    int peer_socket = ::accept(fd, NULL, NULL);
    if (peer_socket == -1) {
        // lo ideal sería devolver Exception si se trata de un
        // error cualquiera, y ClosedSocketException si el socket
        // efectivamente se cerró
        throw ClosedSocketException(ACCEPT_CLOSED_ERROR);
    }
    return peer_socket;
}

ssize_t ProtectedSocket::send(const char* source, const ssize_t len) {
    std::unique_lock<std::mutex> l(m);
    ssize_t total_sent = 0;
    ssize_t last_sent = 0;

    while (total_sent < len) {
        last_sent =
            ::send(fd, &source[total_sent], len - total_sent, MSG_NOSIGNAL);

        if (last_sent == -1) {
            throw Exception(SEND_ERROR);
        } else if (last_sent == 0) {
            return 0;
        } else {
            total_sent += last_sent;
        }
    }

    return total_sent;
}

ssize_t ProtectedSocket::recv(char* buffer, const ssize_t len) {
    std::unique_lock<std::mutex> l(m);
    int total_received = 0;
    int last_received = 0;

    while (total_received < len) {
        last_received =
            ::recv(fd, &buffer[total_received], len - total_received, 0);

        if (last_received == -1) {
            throw Exception(RECV_ERROR);
        } else if (last_received == 0) {
            return 0;
        } else {
            total_received += last_received;
        }
    }

    return total_received;
}

//-----------------------------------------------------------------------------

ssize_t ProtectedSocket::operator<<(uint16_t n) {
    return send((char*)&n, sizeof(n));
}

ssize_t ProtectedSocket::operator>>(uint16_t& n) {
    uint16_t received;
    ssize_t n_received;
    n_received = recv((char*)&received, sizeof(received));
    n = received;
    return n_received;
}

ssize_t ProtectedSocket::operator<<(const std::string& msg) {
    return send(msg.c_str(), msg.size());
}

ssize_t ProtectedSocket::operator<<(char c) {
    return send(&c, sizeof(c));
}

ssize_t ProtectedSocket::operator>>(char& c) {
    return recv(&c, sizeof(c));
}

//-----------------------------------------------------------------------------

void ProtectedSocket::shutdown(const int& channel) {
    std::unique_lock<std::mutex> l(m);

    if (fd_valid) {
        if (::shutdown(fd, channel)) {
            throw Exception(SHUTDOWN_ERROR);
        }
    }
}

void ProtectedSocket::close() {
    std::unique_lock<std::mutex> l(m);
    _closeFdIfValid();
}

ProtectedSocket::~ProtectedSocket() {
    std::unique_lock<std::mutex> l(m);
    _closeFdIfValid();
}

//-----------------------------------------------------------------------------
