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
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HomeView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Conexión
    SocketWrapper& socket;

    // Componentes

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos internos */
    void _init();

    /* Carga la media necesaria */
    void _loadMedia();

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
