#ifndef __HOME_VIEW_H__
#define __HOME_VIEW_H__

//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConstantRateFunc.h"
#include "contexts.h"
#include "defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HomeView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Conexión
    SocketWrapper& socket;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Iteración del loop */
    void _func(const int it) override;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    HomeView(Context& current_context, SocketWrapper& socket);

    /* Destructor */
    ~HomeView();
};

//-----------------------------------------------------------------------------

#endif  // __HOME_VIEW_H__
