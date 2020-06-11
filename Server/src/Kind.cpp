#include <string>

#include "../includes/Kind.h"

Kind::Kind(const KindCfg& data):
                id(data.id),
                name(data.name),
                max_health_factor(data.max_health_factor),
                meditation_factor(data.meditation_factor),
                max_mana_factor(data.max_mana_factor) {}
            
Kind::~Kind() {}

void Kind::doMagic() {
    if (!max_mana_factor) {
        throw Exception("La clase ", this->name, " no puede hacer magia.");
    }
}

void Kind::meditate() {
    if (!meditation_factor) {
        throw Exception("La clase ", this->name, " no puede meditar.");
    }
}
