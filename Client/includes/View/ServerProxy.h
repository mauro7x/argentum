#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

//-----------------------------------------------------------------------------
#include "MapView.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ServerProxy {
   private:
    const MapView& map;

   public:
    /* Constructor */
    ServerProxy(const MapView& map);

    /* Deshabilitamos el constructor por copia. */
    ServerProxy(const ServerProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ServerProxy& operator=(const ServerProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ServerProxy(ServerProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ServerProxy& operator=(ServerProxy&& other) = delete;

    //-------------------------------------------------------------------------
    // Métodos proxy

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ServerProxy();
};

//-----------------------------------------------------------------------------

#endif  // __SERVER_PROXY_H__
