#ifndef __KIND_H__
#define __KIND_H__

#include <string>

#include "../../../Common/includes/types.h"
#include "config_structs.h"

/*
 * Clase:
 *     Mago, clerigo, paladin, guerrero
 *
 * Aportan factores a tener en cuenta en las ecuaciones de
 * vida, mana y meditacion. Ademas, son capaces (o no) de
 * meditar y/o hacer magia, y esto ultimo se ve reflejado
 * en si puede hacer hechizos o no.
 */
class Kind {
   public:
    const Id id;
    const std::string name;

    const unsigned int max_health_factor;
    const unsigned int meditation_factor;
    const unsigned int max_mana_factor;

    Kind(const KindCfg& data);
    ~Kind();

    Kind(const Kind&) = delete;
    Kind& operator=(const Kind&) = delete;
    Kind(Kind&&) = delete;
    Kind& operator=(Kind&&) = delete;

    /* Lanza KindCantDoMagicException si el kind no puede hacer magia. */
    void doMagic() const;

    /* Lanza KindCantMeditateException si el kind no puede meditar. */
    void meditate() const;
};

class KindCantDoMagicException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class KindCantMeditateException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif
