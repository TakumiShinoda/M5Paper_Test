#include "./RectButton.h"


RectButton::RectButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* image, CustomSetups* setup, PriorityNum priority) : Parts(x, y, setup, priority){
    this->_w = w;
    this->_h = h;
    this->_data_image = (uint8_t*)image;
    this->_millis_update_span = 1;

    if(image == nullptr) return;

    Parts::createHash(this->_canvas_hash, RTEPD_SIZE_HASH);
    RTEPD_API::canvasRegister(this->_canvas_hash, this->_w, this->_h);
}

void RectButton::init(){
    Parts::init();
    this->_flag_rendered = false;
    this->_flag_reversed = false;
    this->_flag_pressing = false;
    this->_flag_released = false;
}

void RectButton::update(){
    this->updateWithoutRender();
    
    if(this->_flag_stop_render || this->_flag_disable) return;
    
    RTEPD_API::canvasDraw(this->_canvas_hash, this->_x, this->_y, UPDATE_MODE_DU4);

    this->debugPrint("Update: RectButton");
}

void RectButton::updateWithoutRender(){
    bool IsTouch;
    tp_finger_t Finger;

    if(this->_flag_disable) return;

    this->_flag_released = false;
    IsTouch = RTTP::IsTouch;
    Finger = RTTP::Fingers[0];

    if(!this->_flag_rendered){
        RTEPD_API::canvasSetRawImage(this->_canvas_hash, 0, 0, this->_w, this->_h, this->_data_image);
        this->_flag_rendered = true;

        return;
    }

    if(IsTouch){
        this->debugPrint("touch");

        if(
            (Finger.x >= this->_x) &&
            (Finger.x <= (this->_x + this->_w)) &&
            (Finger.y >= this->_y) &&
            (Finger.y <= (this->_y + this->_h)) &&
            (!this->_flag_reversed)
        ){
            RTEPD_API::canvasSetReverse(this->_canvas_hash);

            this->_flag_reversed = true;
            this->_flag_pressing = true;
            this->_flag_stop_render = false;

            return;
        }else if(
            (
                (Finger.x < this->_x) ||
                (Finger.x > (this->_x + this->_w)) ||
                (Finger.y < this->_y) ||
                (Finger.y > (this->_y + this->_h))
            ) &&
            this->_flag_reversed
        ){
            RTEPD_API::canvasSetReverse(this->_canvas_hash);

            this->_flag_reversed = false;
            this->_flag_pressing = false;
            this->_flag_stop_render = false;

            return;
        }
    }else{
        this->debugPrint("not touch");

        if(this->_flag_pressing) this->_flag_released = true;
        this->_flag_pressing = false;

        if(this->_flag_reversed){
            RTEPD_API::canvasSetReverse(this->_canvas_hash);

            this->_flag_reversed = false;
            this->_flag_stop_render = false;

            return;
        }
    }

    this->_flag_stop_render = true;
}

bool RectButton::isReleased(){
    return this->_flag_released;
}