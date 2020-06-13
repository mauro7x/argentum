#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EventHandler : public Thread {
   private:
    std::atomic_bool& view_running;
    Queue<int*>& requests;

   public:
    /* Constructor */
    EventHandler(std::atomic_bool& view_running, Queue<int*>& requests);

    /* Método de ejecución del hilo */
    void run() override;

    /* Destructor */
    ~EventHandler();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_HANDLER_H__
