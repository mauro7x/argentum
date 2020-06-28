#ifndef __CONFIG_H__
#define __CONFIG_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/**
 * Wrapper para el parser de los archivos json de configuración.
 * Se encarga de parsear el json correspondiente a cada config_struct, para
 * llenar el mismo con la configuración correspondiente, y acceder a la
 * información utilizando un HashMap (O(1)). Indexa el acceso por ids.
 */

template <class T>
class Config {
   private:
    std::unordered_map<Id, T> config;

    /* Parsea el json correspondiente y llena la estructura. Se define
     * polimórficamente para los distintos config_structs */
    void _parseFile();

   public:
    /* Constructor */
    Config() {
        _parseFile();
    }

    /* Deshabilitamos el constructor por copia. */
    Config(const Config&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Config& operator=(const Config&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Config(Config&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Config& operator=(Config&& other) = delete;

    /* Operador para lectura de configuración */
    const T& operator[](const Id id) const {
        if (config.count(id) == 0) {
            throw Exception("Config.cpp operator[]: Unknown id.");
        }

        return config.at(id);
    }

    /* 
     * Agrega al vector ids recibido todas las ids existentes
     * en la configuración.
     */
    void gatherIds(std::vector<Id>& ids) const {
        auto it = this->config.begin();
        while (it != this->config.end()) {
            ids.push_back(it->first);
            ++it;
        }
    }

    /* encontrar si existe tal id en config */
    size_t count(Id id) const {
        return config.count
    }

    /* Conocer la cantidad de Ts que tenemos */
    size_t size() const {
        return config.size();
    }

    /* Destructor */
    ~Config() {}
};

//-----------------------------------------------------------------------------

#endif  // __CONFIG_H__
