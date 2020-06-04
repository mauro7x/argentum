#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapProxy {
   private:
    // atributos
    // metodos privados

   public:
    /* Constructor */
    MapProxy();

    /* Deshabilitamos el constructor por copia. */
    MapProxy(const MapProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapProxy& operator=(const MapProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapProxy(MapProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapProxy& operator=(MapProxy&& other) = delete;

    //-------------------------------------------------------------------------

    /* Metodo */
    void metodo() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapProxy();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
