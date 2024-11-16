#include "BUZZER.hpp"

BUZZER::BUZZER(gpio_num_t gpio, ledc_channel_t channel, ledc_timer_t timer){
    this->gpio = gpio;
    this->channel = channel;
    this->timer = timer;
    ledc_timer_config_t configT = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = this->timer,
        .freq_hz = 100,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ledc_timer_config(&configT);
    ledc_channel_config_t configC = {
        .gpio_num = this->gpio,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = this->channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = this->timer,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&configC);
}

BUZZER::~BUZZER(){
    ledc_stop(LEDC_LOW_SPEED_MODE, this->channel, 0);
}

void BUZZER::play(int note, int duration){
    if (note == NOTE_REST) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, this->channel, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, this->channel);
    } else {
        ledc_set_freq(LEDC_LOW_SPEED_MODE, this->timer, note);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, this->channel, 1023);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, this->channel);
    }
    vTaskDelay(pdMS_TO_TICKS(duration));
    ledc_set_duty(LEDC_LOW_SPEED_MODE, this->channel, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, this->channel);
}

void BUZZER::on() {
    gpio_set_level(this->gpio, true); 
}

void BUZZER::off() {
    gpio_set_level(this->gpio, false); 
}