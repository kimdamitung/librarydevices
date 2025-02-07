#if !defined(_RGB_H_)
#define _RGB_H_

#include <driver/rmt.h>
#include <driver/gpio.h>
#include <vector>

class RGB {
    private:
        int numbers;
        int bits;
        std::vector<uint8_t> buffer;
        rmt_channel_t rmt_channel;
        void sendRMT(const std::vector<uint8_t>& data);
    public:
        RGB(gpio_num_t pin, int numbers, int bits = 3);
        ~RGB();
        void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);
        void fill(uint8_t r, uint8_t g, uint8_t b);
        void show();
};
/*
extern "C" void app_main() {
    RGB led(GPIO_NUM_48, 1);
    while (true){
        led.fill(255, 0, 0);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(0, 255, 0);
        led.show();
    }
}
*/
#endif // _RGB_H_
