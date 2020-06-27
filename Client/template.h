#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Template {
   private:
    //-----------------------------------------------------------------------------
    // Atributos
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Template();

    /* Deshabilitamos el constructor por copia. */
    Template(const Template&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Template& operator=(const Template&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Template(Template&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Template& operator=(Template&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública
    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Template();
};

//-----------------------------------------------------------------------------

#endif  // __TEMPLATE_H__
