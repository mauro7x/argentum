#ifndef __UPDATE_H__
#define __UPDATE_H__

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Update abstracto que se sabe ejecutar. Puede recibir parámetros en el método
 * de ejecución según sea necesario. Tener en cuenta que los atributos
 * particulares que necesite cada comando deben ser agregados como atributos
 * específicos de las clases concretas que hereden de esta. */

class Update {
   public:
    /* Constructor */
    Update() {}

    /* Deshabilitamos el constructor por copia. */
    Update(const Update&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Update& operator=(const Update&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Update(Update&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Update& operator=(Update&& other) = delete;

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del update */
    virtual void operator()(/* agregar parámetros comunes a todos */) = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Update() {}
};

//-----------------------------------------------------------------------------

#endif  // __UPDATE_H__
