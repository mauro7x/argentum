#include "../../includes/Socket/SocketWrapper.h"

//-----------------------------------------------------------------------------
// Métodos privados

SocketWrapper::SocketWrapper(const int fd) : Socket(fd) {}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SocketWrapper::SocketWrapper(const std::string& port,
                             const int max_queued_clients)
    : Socket(port, max_queued_clients) {}

SocketWrapper::SocketWrapper(const std::string& hostname,
                             const std::string& port)
    : Socket(hostname, port) {}

SocketWrapper::SocketWrapper(SocketWrapper&& other)
    : Socket(std::move(other)) {}

SocketWrapper& SocketWrapper::operator=(SocketWrapper&& other) {
    Socket::operator=(std::move(other));
    return *this;
}

SocketWrapper SocketWrapper::accept() const {
    int peer_socket = ::accept(fd, NULL, NULL);
    if (peer_socket == -1) {
        throw ClosedSocketException("Error in function: Socket::accept()");
    }
    return std::move(SocketWrapper(peer_socket));
}

//-----------------------------------------------------------------------------
// Sobrecarga de operadores

ssize_t SocketWrapper::operator<<(uint16_t n) const {
    return send((char*)&n, sizeof(n));
}

ssize_t SocketWrapper::operator>>(uint16_t& n) const {
    uint16_t received;
    ssize_t n_received;
    n_received = recv((char*)&received, sizeof(received));
    n = received;
    return n_received;
}

ssize_t SocketWrapper::operator<<(const std::string& msg) const {
    return send(msg.c_str(), msg.size());
}

ssize_t SocketWrapper::operator<<(char c) const {
    return send(&c, sizeof(c));
}

ssize_t SocketWrapper::operator>>(char& c) const {
    return recv(&c, sizeof(c));
}

//-----------------------------------------------------------------------------

SocketWrapper::~SocketWrapper() {}

//-----------------------------------------------------------------------------
