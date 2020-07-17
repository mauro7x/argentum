#include <string>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Kind.h"
//-----------------------------------------------------------------------------

Kind::Kind(const KindCfg& data):
                id(data.id),
                name(data.name),
                max_health_factor(data.max_health_factor),
                meditation_factor(data.meditation_factor),
                max_mana_factor(data.max_mana_factor) {}
            
Kind::~Kind() {}

//-----------------------------------------------------------------------------

const bool Kind::doMagic() const {
    if (!max_mana_factor)
        return false;
    return true;
}

const bool Kind::meditate() const {
    if (!meditation_factor) 
        return false;
    return true;
}

//-----------------------------------------------------------------------------
