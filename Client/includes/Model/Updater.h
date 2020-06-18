#ifndef __UPDATER_H__
#define __UPDATER_H__

//-----------------------------------------------------------------------------
#include <atomic>
#include <exception>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Update.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Updater : public Thread {
   private:
    const SocketWrapper& socket;        /* SÓLO LECTURA (RECV) */
    NonBlockingQueue<Update*>& updates; /* Updates a recibir */
    std::atomic_bool& exit;             /* flag de ejecución compartido */

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Updater(const SocketWrapper& socket, NonBlockingQueue<Update*>& updates,
            std::atomic_bool& exit);

    /* Deshabilitamos el constructor por copia. */
    Updater(const Updater&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Updater& operator=(const Updater&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Updater(Updater&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Updater& operator=(Updater&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Hilo principal de ejecución */
    void run() override;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Updater();
};

//-----------------------------------------------------------------------------

#endif  // __UPDATER_H__
