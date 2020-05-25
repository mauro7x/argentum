#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Template {
   private:
    // atributos
    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: -
     */
    Template();

    /* Deshabilitamos el constructor por copia. */
    Template(const Template&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Template& operator=(const Template&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Template(Template&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Template& operator=(Template&& other) = delete;

    /**
     * Descripción: destructor.
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void metodo() const;

    /**
     * Descripción: destructor.
     */
    ~Template();
};

//-----------------------------------------------------------------------------

#endif  // __TEMPLATE_H__
