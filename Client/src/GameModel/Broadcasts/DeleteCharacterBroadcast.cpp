#include "../../../includes/GameModel/Broadcasts/DeleteCharacterBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteCharacterBroadcast::DeleteCharacterBroadcast(const InstanceId& id)
    : Broadcast(), id(id) {}

void DeleteCharacterBroadcast::exec(MapView& map, Player& player,
                                    CharacterContainer& characters,
                                    CreatureContainer& creatures) {
    SDL_Point old_pos = characters.getPos(id);
    characters.remove(id);

    map.clearTileOccupant(old_pos.x, old_pos.y);
}

DeleteCharacterBroadcast::~DeleteCharacterBroadcast() {}

//-----------------------------------------------------------------------------
