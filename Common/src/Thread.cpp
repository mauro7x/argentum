#include "../includes/Thread.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Thread::Thread() : thread() {}

Thread::Thread(Thread&& other) {
    this->thread = std::move(other.thread);
}

Thread& Thread::operator=(Thread&& other) {
    this->thread = std::move(other.thread);
    return *this;
}

void Thread::start() {
    thread = std::thread(&Thread::run, this);
}

bool Thread::joinable() {
    return thread.joinable();
}

void Thread::join() {
    if (thread.joinable()) {
        thread.join();
    }
}

Thread::~Thread() {
    join();
}

//-----------------------------------------------------------------------------
