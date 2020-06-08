#include "../includes/Kind.h"

Kind::Kind(KindCfg& data):
                id(data.id),
                name(data.name),
                health(data.health),
                meditation(data.meditation),
                mana(data.mana) {}
            
Kind::~Kind() {}

//IMPLEMENTAR METODOS.
