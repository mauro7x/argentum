#include "../../includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include <endian.h>
//-----------------------------------------------------------------------------
// Métodos privados

SocketWrapper::SocketWrapper(const int fd) : Socket(fd) {}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SocketWrapper::SocketWrapper() : Socket() {}

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
    if (!fd_valid) {
        throw Exception("Invalid socket file descriptor.");
    }

    int peer_socket = ::accept(fd, NULL, NULL);
    if (peer_socket == -1) {
        throw ClosedSocketException("Error in function: Socket::accept()");
    }
    return std::move(SocketWrapper(peer_socket));
}

//-----------------------------------------------------------------------------
// Sobrecarga de operadores

size_t SocketWrapper::operator<<(char c) const {
    return send(&c, sizeof(c));
}

size_t SocketWrapper::operator>>(char& c) const {
    return recv(&c, sizeof(c));
}

size_t SocketWrapper::operator<<(const uint8_t& n) const {
    return send((char*)&n, sizeof(n));
}

size_t SocketWrapper::operator>>(uint8_t& n) const {
    uint8_t received;
    size_t n_received;
    n_received = recv((char*)&received, sizeof(received));
    n = received;
    return n_received;
}

size_t SocketWrapper::operator<<(const uint32_t& n) const {
    uint32_t _n = htole32(n);
    return send((char*)&_n, sizeof(_n));
}

size_t SocketWrapper::operator>>(uint32_t& n) const {
    uint32_t _received;
    size_t n_received;
    n_received = recv((char*)&_received, sizeof(_received));
    n = le32toh(_received);
    return n_received;
}

size_t SocketWrapper::operator<<(const std::string& s) const {
    size_t sent = 0, last_sent = 0;

    // Primero enviamos la longitud
    uint32_t size = s.size();
    sent += this->operator<<(size);
    if (sent != sizeof(size)) {
        return 0;
    }

    // Enviamos el string
    last_sent = send(s.c_str(), size);
    if (last_sent) {
        sent += last_sent;
    } else {
        return 0;
    }

    return sent;
}

size_t SocketWrapper::operator>>(std::string& s) const {
    size_t received = 0, last_received = 0;
    s.clear();
    s.shrink_to_fit();

    // Primero recibimos la longitud
    uint32_t size;
    received += this->operator>>(size);
    if (received != sizeof(size)) {
        return 0;
    }

    // Recibimos el string
    s.reserve(size);
    char byte;
    for (unsigned int i = 0; i < size; i++) {
        last_received += this->operator>>(byte);
        if (last_received) {
            received += last_received;
            s += byte;
        } else {
            return 0;
        }
    }

    return received;
}

size_t SocketWrapper::operator<<(const std::vector<uint8_t>& v) const {
    size_t sent = 0, last_sent = 0;

    // Primero enviamos la longitud
    uint32_t size = v.size();
    sent += this->operator<<(size);
    if (sent != sizeof(size)) {
        return 0;
    }

    // Enviamos el vector
    uint8_t byte;
    for (unsigned int i = 0; i < size; i++) {
        byte = v.at(i);
        last_sent = this->operator<<(byte);
        if (last_sent) {
            sent += last_sent;
        } else {
            return 0;
        }
    }

    return sent;
}

size_t SocketWrapper::operator>>(std::vector<uint8_t>& v) const {
    size_t received = 0, last_received = 0;
    v.clear();
    v.shrink_to_fit();

    // Primero recibimos la longitud
    uint32_t size;
    received += this->operator>>(size);
    if (received != sizeof(size)) {
        return 0;
    }

    // Recibimos el vector
    v.reserve(size);
    uint8_t byte;
    for (unsigned int i = 0; i < size; i++) {
        last_received += this->operator>>(byte);
        if (last_received) {
            received += last_received;
            v.push_back(byte);
        } else {
            return 0;
        }
    }

    return received;
}

//-----------------------------------------------------------------------------

SocketWrapper::~SocketWrapper() {}

//-----------------------------------------------------------------------------
