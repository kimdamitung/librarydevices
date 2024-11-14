#if !defined(_ST7735_H_)
#define _ST7735_H_

#include <stdbool.h>
#include <stdint.h>
#include <cstring>
#include <math.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define IS_160x128 = 1

typedef struct{
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
}fonts;

extern fonts font7x10;
extern fonts font11x18;
extern fonts font16x26;
extern const uint16_t data[][128];

typedef enum {
    GAMMA_10 = 0x01,
    GAMMA_25 = 0x02,
    GAMMA_22 = 0x04,
    GAMMA_18 = 0x08
} gamma_t;

class ST7735{
    private:
        const uint16_t MADCTL_MY        = 0x80;
        const uint16_t MADCTL_MX        = 0x40;
        const uint16_t MADCTL_MV        = 0x20;
        const uint16_t MADCTL_ML        = 0x10;
        const uint16_t MADCTL_RGB       = 0x00;
        const uint16_t MADCTL_BGR       = 0x08;
        const uint16_t MADCTL_MH        = 0x04;
        const int WIDTH                 = 128;
        const int HEIGHT                = 160;
        const int XSTART                = 0;
        const int YSTART                = 0;
        const uint16_t ROTATION         = (MADCTL_MX | MADCTL_MY);
        const uint16_t NOP              = 0x00;
        const uint16_t SWRESET          = 0x01;
        const uint16_t RDDID            = 0x04;
        const uint16_t RDDST            = 0x09;
        const uint16_t SLPIN            = 0x10;
        const uint16_t SLPOUT           = 0x11;
        const uint16_t PTLON            = 0x12;
        const uint16_t NORON            = 0x13;
        const uint16_t INVOFF           = 0x20;
        const uint16_t INVON            = 0x21;
        const uint16_t GAMSET           = 0x26;
        const uint16_t DISPOFF          = 0x28;
        const uint16_t DISPON           = 0x29;
        const uint16_t CASET            = 0x2A;
        const uint16_t RASET            = 0x2B;
        const uint16_t RAMWR            = 0x2C;
        const uint16_t RAMRD            = 0x2E;
        const uint16_t PTLAR            = 0x30;
        const uint16_t COLMOD           = 0x3A;
        const uint16_t MADCTL           = 0x36;
        const uint16_t FRMCTR1          = 0xB1;
        const uint16_t FRMCTR2          = 0xB2;
        const uint16_t FRMCTR3          = 0xB3;
        const uint16_t INVCTR           = 0xB4;
        const uint16_t DISSET5          = 0xB6;
        const uint16_t PWCTR1           = 0xC0;
        const uint16_t PWCTR2           = 0xC1;
        const uint16_t PWCTR3           = 0xC2;
        const uint16_t PWCTR4           = 0xC3;
        const uint16_t PWCTR5           = 0xC4;
        const uint16_t VMCTR1           = 0xC5;
        const uint16_t RDID1            = 0xDA;
        const uint16_t RDID2            = 0xDB;
        const uint16_t RDID3            = 0xDC;
        const uint16_t RDID4            = 0xDD;
        const uint16_t PWCTR6           = 0xFC;
        const uint16_t GMCTRP1          = 0xE0;
        const uint16_t GMCTRN1          = 0xE1;
        const uint16_t BLACK            = 0x0000;
        const uint16_t BLUE             = 0x001F;
        const uint16_t RED              = 0xF800;
        const uint16_t GREEN            = 0x07E0;
        const uint16_t CYAN             = 0x07FF;
        const uint16_t MAGENTA          = 0xF81F;
        const uint16_t YELLOW           = 0xFFE0;
        const uint16_t WHITE            = 0xFFFF;
        const uint16_t DELAY            = 0x80;
        gpio_num_t cs;
        gpio_num_t dc;
        gpio_num_t rst;
        spi_host_device_t host;
        spi_device_handle_t spi;
        static bool spi_bus_initialized; 
        void select();
        void unselect();
        void reset();
        void write(uint8_t commands);
        void write(const uint8_t *buffer, size_t size);
        void commands(const uint8_t *address);
        void setAddressWindows(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

    public:
        ST7735(gpio_num_t cs, gpio_num_t dc, gpio_num_t rst, spi_host_device_t host, gpio_num_t mosi, gpio_num_t sck, gpio_num_t miso = GPIO_NUM_NC);
        ~ST7735();
        void drawPixels(uint16_t x, uint16_t y, uint16_t color);
        void characters(uint16_t x, uint16_t y, char ch, fonts font, uint16_t color, uint16_t background);
        void strings(uint16_t x, uint16_t y, const char* string, fonts font, uint16_t color, uint16_t background);
        void rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
        void setImagesFullScreen(const uint16_t data[160][128]);
};

#endif // _ST7735_H_