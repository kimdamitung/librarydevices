#if !defined(_ST7735_H_)
#define _ST7735_H_

#include <stdint.h>
#include "driver/spi_common.h"

#define MADCTL_MY   0x80
#define MADCTL_MX   0x40
#define MADCTL_MV   0x20
#define MADCTL_ML   0x10
#define MADCTL_RGB  0x00
#define MADCTL_BGR  0x08
#define MADCTL_MH   0x04
#define IS_160X128  1
// #define WIDTH     128
// #define HEIGHT    160
#define WIDTH       160
#define HEIGHT      128
#define XSTART      0
#define YSTART      0
// #define ROTATION  (MADCTL_MX | MADCTL_MY)
#define ROTATION    (MADCTL_MY | MADCTL_MV)
#define NOP         0x00
#define SWRESET     0x01
#define RDDID       0x04
#define RDDST       0x09
#define SLPIN       0x10
#define SLPOUT      0x11
#define PTLON       0x12
#define NORON       0x13
#define INVOFF      0x20
#define INVON       0x21
#define GAMSET      0x26
#define DISPOFF     0x28
#define DISPON      0x29
#define CASET       0x2A
#define RASET       0x2B
#define RAMWR       0x2C
#define RAMRD       0x2E
#define PTLAR       0x30
#define COLMOD      0x3A
#define MADCTL      0x36
#define FRMCTR1     0xB1
#define FRMCTR2     0xB2
#define FRMCTR3     0xB3
#define INVCTR      0xB4
#define DISSET5     0xB6
#define PWCTR1      0xC0
#define PWCTR2      0xC1
#define PWCTR3      0xC2
#define PWCTR4      0xC3
#define PWCTR5      0xC4
#define VMCTR1      0xC5
#define RDID1       0xDA
#define RDID2       0xDB
#define RDID3       0xDC
#define RDID4       0xDD
#define PWCTR6      0xFC
#define GMCTRP1     0xE0
#define GMCTRN1     0xE1
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define GAMMA_10    0x01
#define GAMMA_25    0x02
#define GAMMA_22    0x04
#define GAMMA_18    0x08
#define DELAY       0x80

const uint8_t init_cmds1[] = {
    15,
    SWRESET, DELAY,
    150,
    SLPOUT, DELAY,
    255,
    FRMCTR1, 3, 0x01, 0x2C, 0x2D,
    FRMCTR2, 3, 0x01, 0x2C, 0x2D,
    FRMCTR3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,
    INVCTR, 1, 0x07,
    PWCTR1, 3, 0xA2, 0x02, 0x84,
    PWCTR2, 1, 0xC5,
    PWCTR3, 2, 0x0A, 0x00,
    PWCTR4, 2, 0x8A, 0x2A,
    PWCTR5, 2, 0x8A, 0xEE,
    VMCTR1, 1, 0x0E,
    INVOFF, 0,
    MADCTL, 1, ROTATION,
    COLMOD, 1, 0x05
};

const uint8_t init_cmds2[] = {
    3,
    CASET, 4, 0x00, 0x00, 0x00, 0x4F,
    RASET, 4, 0x00, 0x00, 0x00, 0x9F,
    INVON, 0
};

const uint8_t init_cmds3[] = {
    4,
    GMCTRP1, 16, 0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,
    GMCTRN1, 16, 0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,
    NORON, DELAY,
    10,
    DISPON, DELAY,
    100
};

class st7735{
    private:
        /* data */
    public:
        st7735(/* args */);
        ~st7735();
};

#endif // _ST7735_H_