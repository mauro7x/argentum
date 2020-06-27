#ifndef __COLORS_H__
#define __COLORS_H__

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : r(r), g(g), b(b), a(a) {}
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//#define ERROR_MSG_COLOR Color(199, 12, 12, 255)
//#define WARNING_MSG_COLOR Color(255, 109, 5, 255)
//#define INFO_MSG_COLOR Color(255, 255, 255, 255)
//#define SUCCESS_MSG_COLOR Color(120, 235, 91, 255)
//#define LIST_MSG_COLOR Color(247, 223, 134, 255)

//#define GENERAL_MSG_COLOR Color(255, 255, 255, 255)
//#define PRIVATE_MSG_COLOR Color(250, 122, 199, 255)

//#define USER_CMD_MSG_COLOR Color(110, 110, 110, 255)
//-----------------------------------------------------------------------------

#endif  // __COLORS_H__