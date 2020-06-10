#include <string>

#include "../includes/Kind.h"

Kind::Kind(const KindCfg& data):
                id(data.id),
                name(data.name),
                health(data.health),
                meditation(data.meditation),
                mana(data.mana) {}
            
Kind::~Kind() {}

void Kind::doMagic() {
    if (!mana) {
        throw Exception("La clase ", this->name, " no puede hacer magia.");
    }
}

void Kind::meditate() {
    if (!meditation) {
        throw Exception("La clase ", this->name, " no puede meditar.");
    }
}
