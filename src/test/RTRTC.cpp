#include "./RTRTC.h"

rtc_time_t RTRTC::Now = {0, 0, 0};

void RTRTC::touchProcess(void* params){
    while(true){
        xSemaphoreTake(Mutexes::i2c, portMAX_DELAY);
        M5.RTC.getTime(&RTRTC::Now);
        xSemaphoreGive(Mutexes::i2c);

        vTaskDelay(100);
    }
}