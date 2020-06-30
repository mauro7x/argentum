#ifndef __ROUND_ROBIN_LIST_H__
#define __ROUND_ROBIN_LIST_H__

//-----------------------------------------------------------------------------
#include <iterator>
#include <list>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class T>
class RoundRobinList {
   private:
    std::list<T> content;
    typename std::list<T>::iterator it;

   public:
    /* Constructor */
    RoundRobinList() : it(content.begin()) {}

    /* Deshabilitamos el constructor por copia. */
    RoundRobinList(const RoundRobinList&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    RoundRobinList& operator=(const RoundRobinList&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    RoundRobinList(RoundRobinList&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    RoundRobinList& operator=(RoundRobinList&& other) = delete;

    //-------------------------------------------------------------------------
    // Métodos de la API pública

    /* Obtiene el último elemento */
    T& back() {
        return content.back();
    }

    /* Construye in-place al final */
    template <typename... Args>
    void emplaceBack(Args&&... args) {
        content.emplace_back(std::forward<Args>(args)...);
        it = content.begin();
    }

    /* Agrega un elemento al final */
    void pushBack(const T& value) {
        content.push_back(value);
        it = content.begin();
    }

    /* Devuelve si está vacía */
    bool empty() {
        return content.empty();
    }

    /* Devuelve el tamaño */
    size_t size() {
        return content.size();
    }

    /* Avanza el iterador */
    void next() {
        if (content.size() <= 1) {
            return;
        }

        it++;
        if (it == content.end()) {
            it = content.begin();
        }
    }

    /* Retrocede el iterador */
    void prev() {
        if (content.size() <= 1) {
            return;
        }

        if (it != content.begin()) {
            it--;
        } else {
            it = content.end();
            it--;
        }
    }

    /* Devuelve un iterador al inicio de la lista */
    typename std::list<T>::const_iterator begin() const {
        return content.begin();
    }

    /* Devuelve un iterador al final de la lista */
    typename std::list<T>::const_iterator end() const {
        return content.end();
    }

    /* Settea el iterador en el principio */
    void reset() {
        it = content.begin();
    }

    /* Obtiene el valor actual */
    T& getCurrentValue() const {
        return *it;
    }

    //-------------------------------------------------------------------------

    /* Destructor */
    ~RoundRobinList() {}
};

//-----------------------------------------------------------------------------

#endif  // __ROUND_ROBIN_LIST_H__