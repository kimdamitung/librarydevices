#if !defined(_ESPNOW_H_)
#define _ESPNOW_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(addr) (addr)[0], (addr)[1], (addr)[2], (addr)[3], (addr)[4], (addr)[5]

struct data_t;

class ESPNOW{
    private:
        /* data */
        uint8_t *address[6];
        int channel;
        bool mode; /*true is master or false is slave*/
        data_t* message;
        static ESPNOW* instance;
        static void onConnect(const uint8_t* address, esp_now_send_status_t status);
        static void onMessage(const esp_now_recv_info_t* infomation, const uint8_t* data, int lenght);
    public:
        ESPNOW(const uint8_t* address, int channel, bool mode = true);
        ~ESPNOW();
        void setMessage(const uint8_t* data, size_t size);
        const data_t& getMessage() const;
        void sConnect(const uint8_t* address, esp_now_send_status_t status);
        void sMessage(const esp_now_recv_info_t* infomation, const uint8_t* data, int lenght);
};

#endif // _ESPNOW_H_
