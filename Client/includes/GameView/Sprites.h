#ifndef __SPRITES_H__
#define __SPRITES_H__

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct UnitSprite {
    Id id = 0;
    uint16_t clip_w = 0, clip_h = 0;
    uint8_t up_row = 0, up_col = 0, up_clips = 0;
    uint8_t down_row = 0, down_col = 0, down_clips = 0;
    uint8_t left_row = 0, left_col = 0, left_clips = 0;
    uint8_t right_row = 0, right_col = 0, right_clips = 0;
    uint8_t change_every_n_frames = 0;
    Texture texture;

    UnitSprite(Id id) : id(id), texture() {}
};

struct ItemSprite {
    Id id = 0;
    Texture texture;

    ItemSprite(Id id) : id(id), texture() {}
};

//-----------------------------------------------------------------------------

#endif  // __SPRITES_H__
