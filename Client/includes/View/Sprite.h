#ifndef __SPRITE_H__
#define __SPRITE_H__

//-----------------------------------------------------------------------------
#include <cstdint>

#include "../../../Common/includes/types.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Sprite {
    Id id;
    uint16_t clip_w, clip_h;
    uint8_t up_row, up_col, up_clips;
    uint8_t down_row, down_col, down_clips;
    uint8_t left_row, left_col, left_clips;
    uint8_t right_row, right_col, right_clips;
    uint8_t change_every_n_frames;
    Texture texture;
};

//-----------------------------------------------------------------------------

#endif  // __SPRITE_H__
