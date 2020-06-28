#ifndef __CONNECTION_VIEW_H__
#define __CONNECTION_VIEW_H__

//-----------------------------------------------------------------------------
#include "../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConstantRateFunc.h"
#include "contexts.h"
#include "defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ConnectionView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Conexión
    const SocketWrapper& socket;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Iteración del loop */
    void _func(const int it) override;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    ConnectionView(Context& current_context, const SocketWrapper& socket);

    /* Destructor */
    ~ConnectionView();
};

//-----------------------------------------------------------------------------

#endif  // __CONNECTION_VIEW_H__
