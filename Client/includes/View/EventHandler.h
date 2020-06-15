#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Thread.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EventHandler : public Thread {
   private:
    Window& g_window;
    std::atomic_bool& view_running;
    Queue<int*>& requests;

   public:
    /* Constructor */
    EventHandler(Window& window, std::atomic_bool& view_running,
                 Queue<int*>& requests);

    /* Método de ejecución del hilo */
    void run() override;

    /* Destructor */
    ~EventHandler();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_HANDLER_H__
