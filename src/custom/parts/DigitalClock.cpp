#include "./DigitalClock.h"

DigitalClock::DigitalClock(uint16_t x, uint16_t y, CustomSetups* setup, PriorityNum priority): Parts(x, y, setup, priority){
    this->_millis_update_span = 700;
    this->_w = 90;
    this->_h = 26;

    Parts::createHash(this->_canvas_hash, RTEPD_SIZE_HASH);
    RTEPD_API::canvasRegister(this->_canvas_hash, this->_w, this->_h);
}

void DigitalClock::update(){
    Serial.println("Digit");
    if(!this->_setup->isAvailableEPD()) return;
    if(!this->isReadyToUpdate()) return;

    Serial.println("Digit1");
    this->updateWithoutRender();

    Serial.println("Digit2");
    RTEPD_API::canvasDraw(this->_canvas_hash, this->_x, this->_y, UPDATE_MODE_DU4);

    Serial.println("Digit3");
    this->_show_Separation = !this->_show_Separation;
    Serial.println("Digit4");
    this->setLastUpdate();
    Serial.println("Digit5");
}

void DigitalClock::updateWithoutRender(){
    char* Str_Now = (char*)malloc(sizeof(char) * 6);
    rtc_time_t Now = RTRTC::Now;

    if(this->_show_Separation) sprintf(Str_Now, "%02hhu:%02hhu", Now.hour, Now.min);
    else sprintf(Str_Now, "%02hhu %02hhu", Now.hour, Now.min);

    RTEPD_API::canvasSetText(this->_canvas_hash, 0, 0, 3, Str_Now);
}