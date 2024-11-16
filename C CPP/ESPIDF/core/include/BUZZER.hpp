#if !defined(_BUZZER_H_)
#define _BUZZER_H_

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NOTE_C4     262
#define NOTE_D4     294
#define NOTE_E4     330
#define NOTE_F4     349
#define NOTE_G4     392
#define NOTE_A4     440
#define NOTE_B4     494
#define NOTE_C5     523
#define NOTE_D5     587
#define NOTE_E5     659
#define NOTE_F5     698
#define NOTE_G5     784
#define NOTE_REST   0

class BUZZER{
    private:
        /* data */
        gpio_num_t gpio;
        ledc_channel_t channel;
        ledc_timer_t timer;
    public:
        BUZZER(gpio_num_t gpio, ledc_channel_t channel = LEDC_CHANNEL_0, ledc_timer_t timer = LEDC_TIMER_0);
        ~BUZZER();
        void play(int note, int duration);
        void on();
        void off();
};

#endif // _BUZZER_H_
