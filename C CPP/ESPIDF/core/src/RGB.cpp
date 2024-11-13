#include "RGB.hpp"

RGB::RGB(gpio_num_t pin, int numbers, int bits){
    this->numbers = numbers;
    this->bits = bits;
    this->buffer.resize(numbers * bits);
    this->rmt_channel = RMT_CHANNEL_0;
    rmt_config_t config = {
        .rmt_mode = RMT_MODE_TX,
        .channel = this->rmt_channel,
        .gpio_num = pin,
        .mem_block_num = 1,
        .tx_config = {
            .loop_en = false,
            .idle_output_en = true
        }
    };
    config.clk_div = 2;
    config.tx_config.carrier_en = false;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.carrier_level = RMT_CARRIER_LEVEL_LOW;
    rmt_config(&config);
    rmt_driver_install(this->rmt_channel, 0, 0);
}

RGB::~RGB() {
    rmt_driver_uninstall(this->rmt_channel);
}

void RGB::setPixel(int index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= this->numbers || index < 0)
        return;
    int offset = index * this->bits;
    buffer[offset + 1] = r;
    buffer[offset + 0] = g;
    buffer[offset + 2] = b;
}

void RGB::fill(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < this->numbers; ++i)
        setPixel(i, r, g, b);
}

void RGB::show() {
    sendRMT(this->buffer);
}

void RGB::sendRMT(const std::vector<uint8_t>& data) {
    const int bits_per_byte = 8;
    const int duration_t0h = 14;
    const int duration_t0l = 52;
    const int duration_t1h = 52;
    const int duration_t1l = 14;
    rmt_item32_t items[data.size() * bits_per_byte];
    int item_idx = 0;
    for (auto byte : data) {
        for (int bit = bits_per_byte - 1; bit >= 0; --bit) {
            if (byte & (1 << bit)) {
                items[item_idx].level0 = 1;
                items[item_idx].duration0 = duration_t1h;
                items[item_idx].level1 = 0;
                items[item_idx].duration1 = duration_t1l;
            } else {
                items[item_idx].level0 = 1;
                items[item_idx].duration0 = duration_t0h;
                items[item_idx].level1 = 0;
                items[item_idx].duration1 = duration_t0l;
            }
            item_idx++;
        }
    }
    rmt_write_items(this->rmt_channel, items, item_idx, true);
    rmt_wait_tx_done(this->rmt_channel, portMAX_DELAY);
}