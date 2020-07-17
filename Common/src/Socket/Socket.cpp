#include "../../includes/Socket/Socket.h"

//-----------------------------------------------------------------------------
// Métodos protegidos

Socket::Socket(const int fd) : fd(fd), fd_valid(true) {}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos privados

void Socket::_setServerAddress(const std::string& port,
                               addrinfo** address) const {
    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));

    // Filtros que nos interesan:
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Servidor
    hints.ai_protocol = 0;

    if (::getaddrinfo(0, &port[0], &hints, address)) {
        throw Exception("Error in function: Socket::_setServerAddress()");
    }
}

void Socket::_setClientAddresses(const std::string& hostname,
                                 const std::string& port,
                                 addrinfo** addresses) const {
    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));

    // Filtros que nos interesan:
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = 0;               // Cliente
    hints.ai_protocol = 0;

    if (::getaddrinfo(&hostname[0], &port[0], &hints, addresses)) {
        throw Exception("Error in function: Socket::_setClientAddresses()");
    }
}

void Socket::_setFd(addrinfo* address) {
    fd = ::socket(address->ai_family, address->ai_socktype,
                  address->ai_protocol);

    if (fd == -1) {
        throw Exception("Error in function: Socket::_setFd()");
    } else {
        fd_valid = true;
    }
}

void Socket::_fixTimeWait(addrinfo* address) const {
    int val = 1;
    if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) {
        throw Exception("Error in function: Socket::_fixTimeWait()");
    }
}

void Socket::_bind(addrinfo* address) const {
    int s;
    s = bind(fd, address->ai_addr, address->ai_addrlen);

    if (s == -1) {
        throw Exception("Error in function: Socket::_bind()");
    }
}

void Socket::_listen(const int max_queued_clients) const {
    if (listen(fd, max_queued_clients)) {
        throw Exception("Error in function: Socket::_listen()");
    }
}

void Socket::_tryToConnectTo(addrinfo* addresses) {
    addrinfo* ptr;
    try {
        for (ptr = addresses; ((ptr != NULL) && (fd_valid == false));
             ptr = ptr->ai_next) {
            _setFd(ptr);
            if (::connect(fd, ptr->ai_addr, ptr->ai_addrlen)) {
                _closeFdIfValid();
            }
        }
    } catch (const Exception& e) {
        throw e;
    }

    if (!fd_valid) {
        throw Exception(
            "Error in function: Socket::_tryToConnectTo(). "
            "Client ran out of addresses to try.");
    }
}

void Socket::_closeFdIfValid() {
    if (fd_valid) {
        fd_valid = false;
        ::close(fd);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Socket::Socket() : fd(-1), fd_valid(false) {}

Socket::Socket(const std::string& hostname, const std::string& port)
    : fd(-1), fd_valid(false) {
    // Método para el cliente. Conectarse a un servidor.
    addrinfo* addresses;
    _setClientAddresses(hostname, port, &addresses);

    try {
        _tryToConnectTo(addresses);
    } catch (const Exception& e) {
        ::freeaddrinfo(addresses);
        throw e;
    }

    ::freeaddrinfo(addresses);
}

Socket::Socket(const std::string& port, const int max_queued_clients)
    : fd(-1), fd_valid(false) {
    // Método para el servidor. Abrir servidor.
    addrinfo* address;
    _setServerAddress(port, &address);

    try {
        _setFd(address);
        _fixTimeWait(address);
        _bind(address);
    } catch (const Exception& e) {
        ::freeaddrinfo(address);
        throw e;
    }

    ::freeaddrinfo(address);
    _listen(max_queued_clients);
}

Socket::Socket(Socket&& other) {
    this->fd = other.fd;
    this->fd_valid = other.fd_valid;
    other.fd = -1;
    other.fd_valid = false;
}

Socket& Socket::operator=(Socket&& other) {
    this->fd = other.fd;
    this->fd_valid = other.fd_valid;
    other.fd = -1;
    other.fd_valid = false;
    return *this;
}

Socket Socket::accept() const {
    if (!fd_valid) {
        throw Exception("Invalid socket file descriptor: accept.");
    }

    int peer_socket = ::accept(fd, NULL, NULL);
    if (peer_socket == -1) {
        throw ClosedSocketException("Error in function: Socket::accept()");
    }
    return std::move(Socket(peer_socket));
}

size_t Socket::send(const char* source, const size_t len) const {
    if (!fd_valid) {
        throw Exception("Invalid socket file descriptor: send.");
    }

    size_t total_sent = 0;
    ssize_t last_sent = 0;

    while (total_sent < len) {
        last_sent =
            ::send(fd, &source[total_sent], len - total_sent, MSG_NOSIGNAL);

        if (last_sent < 0) {
            throw Exception("Error in function: Socket::send()");
        } else if (last_sent == 0) {
            return total_sent;
        } else {
            total_sent += (size_t)last_sent;
        }
    }

    return total_sent;
}

size_t Socket::recv(char* buffer, const size_t len) const {
    if (!fd_valid) {
        throw Exception("Invalid socket file descriptor: recv.");
    }

    size_t total_received = 0;
    ssize_t last_received = 0;

    while (total_received < len) {
        last_received =
            ::recv(fd, &buffer[total_received], len - total_received, 0);

        if (last_received < 0) {
            throw Exception("Error in function: Socket::recv()");
        } else if (last_received == 0) {
            return total_received;
        } else {
            total_received += (size_t)last_received;
        }
    }

    return total_received;
}

void Socket::shutdown(const int& channel) const {
    if (fd_valid) {
        if (::shutdown(fd, channel)) {
            throw Exception("Error in function: Socket::shutdown()");
        }
    }
}

void Socket::close() {
    _closeFdIfValid();
}

Socket::~Socket() {
    _closeFdIfValid();
}

//-----------------------------------------------------------------------------
