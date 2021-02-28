#include "./RTTP.h"

bool RTTP::IsTouch = false;
Vector2 RTTP::Pos = {0, 0};
std::vector<tp_finger_t> RTTP::Fingers;

void touchProcess(void* params){
    while(true){
        
        xSemaphoreTake(Mutexes::i2c, portMAX_DELAY);
        RTTP::_touchProcess();
        xSemaphoreGive(Mutexes::i2c);

        // vTaskDelay(10);
    }
}

void RTTP::_touchProcess(){
    tp_finger_t finger;

    if(!M5.TP.avaliable()) return;
    
    M5.TP.update();
    RTTP::IsTouch = !M5.TP.isFingerUp();

    if(!RTTP::IsTouch) return;
    
    std::vector<tp_finger_t>().swap(RTTP::Fingers);
    for(uint8_t i = 0; i < M5.TP.getFingerNum(); i++){
        RTTP::Fingers.push_back(M5.TP.readFinger(i));
    }

    RTTP::Pos.x = finger.x;
    RTTP::Pos.y = finger.y;
}