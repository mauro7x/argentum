#include <string>

#include "../../includes/Model/Kind.h"

Kind::Kind(const KindCfg& data):
                id(data.id),
                name(data.name),
                max_health_factor(data.max_health_factor),
                meditation_factor(data.meditation_factor),
                max_mana_factor(data.max_mana_factor) {}
            
Kind::~Kind() {}

void Kind::doMagic() const {
    if (!max_mana_factor)
        throw KindCantDoMagicException();
}

void Kind::meditate() const {
    if (!meditation_factor) 
        throw KindCantMeditateException();
}

const char* KindCantDoMagicException::what() const noexcept {
    return "Tu clase no puede hacer magia.";
}

const char* KindCantMeditateException::what() const noexcept {
    return "Tu clase no puede meditar.";
}
