#ifndef BUTTON_H
#define BUTTON_H

#include "./Parts.h"
#include "../../test/RTEPD.h"
#include "../../test/RTTP.h"

class RectButton: public Parts{
    public:
        RectButton(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*, CustomSetups*, PriorityNum = 0);
        void init();
        void update();
        void updateWithoutRender();
        bool isReleased();

    private:
        M5EPD_Canvas _canvas_button = M5EPD_Canvas(&M5.EPD);
        char _canvas_hash[RTEPD_SIZE_HASH] = "";
        uint8_t* _data_image;
        bool _flag_rendered = false;
        bool _flag_reversed = false;
        bool _flag_pressing = false;
        bool _flag_released = false;
};

#endif