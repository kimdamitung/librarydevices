#include "ESPNOW.hpp"

ESPNOW* ESPNOW::instance = nullptr;

void ESPNOW::onConnect(const uint8_t* address, esp_now_send_status_t status) {
    if (instance) {
        instance->sConnect(address, status);
    }
}

void ESPNOW::onMessage(const esp_now_recv_info_t* infomation, const uint8_t* data, int lenght) {
    if (instance) {
        instance->sMessage(infomation, data, lenght);
    }
}

void ESPNOW::sConnect(const uint8_t* address, esp_now_send_status_t status) {
    printf("ESPNOW init %s status %s\n", this->mode == true ? "master" : "slave", status == ESP_NOW_SEND_SUCCESS ? "success" : "fail");
}

void ESPNOW::sMessage(const esp_now_recv_info_t* infomation, const uint8_t* data, int lenght) {
    memcpy(&this->message, data, sizeof(this->message));
    printf("MAC: " MACSTR "\n", MAC2STR(infomation->src_addr));
}

ESPNOW::ESPNOW(const uint8_t* address, int channel, bool mode){
    memcpy(this->address, address, 6);
    this->channel = channel;
    this->mode = mode;
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_now_peer_info_t infomation;
    if(this->mode == true){
        esp_now_register_send_cb(this->onConnect);
    }else{
        esp_now_register_recv_cb(this->onMessage);
    }
    memcpy(infomation.peer_addr, this->address, 6);
    infomation.channel = this->channel;
    infomation.encrypt = false;
    esp_now_add_peer(&infomation);
}

ESPNOW::~ESPNOW(){
    if (this->message != nullptr) {
        delete this->message;
        this->message = nullptr;
    }
    esp_now_deinit();
    instance = nullptr;
}

void ESPNOW::setMessage(const uint8_t* data, size_t size){
    memcpy(&this->message, data, size);
}

const data_t& ESPNOW::getMessage() const {
    return *this->message;
}