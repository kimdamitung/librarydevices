#if !defined(_FONTS_H_)
#define _FONTS_H_

#include <stdint.h>

typedef struct{
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
}fonts;

extern fonts font_7x10;
extern fonts font_11x18;
extern fonts font_16x26;

#endif // _FONTS_H_
