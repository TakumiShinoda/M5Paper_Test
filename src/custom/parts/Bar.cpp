#include "./Bar.h"

Bar::Bar(uint16_t x, uint16_t y, CustomSetups *setup, PriorityNum priority) : Parts(x, y, setup, priority){
    this->_w = 540;
    this->_h = 1;
    this->_millis_update_span = 0xFFFFFFFF;

    Parts::createHash(this->_canvas_hash, RTEPD_SIZE_HASH);
    RTEPD_API::canvasRegister(this->_canvas_hash, 540, 1);
}

void Bar::update(){
    // Serial.println("Bar");
    if(!this->_setup->isAvailableEPD()) return;
    if(this->_flag_rendered) return;

    this->updateWithoutRender();
    
    RTEPD_API::canvasDraw(this->_canvas_hash, 0, 50, UPDATE_MODE_DU4);

    this->_flag_rendered = true;
    this->debugPrint("Update: Bar");
}

void Bar::updateWithoutRender(){
    RTEPD_API::canvasSetRect(this->_canvas_hash, 0, 0, 540, 1, 0xFFFF);
}