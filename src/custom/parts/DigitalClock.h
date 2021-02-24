#ifndef DIGITAL_CLOCK_H
#define DIGITAL_CLOCK_H

#include <M5EPD.h>

#include "./Parts.h"
#include "../../test/RTEPD.h"
#include "../../test/RTRTC.h"

class DigitalClock: public Parts{
    public:
        DigitalClock(uint16_t, uint16_t, CustomSetups*, PriorityNum = 0);
        void update();
        void updateWithoutRender();

    private:
        M5EPD_Canvas _canvas_clock = M5EPD_Canvas(&M5.EPD);
        char _canvas_hash[RTEPD_SIZE_HASH] = "";
        bool _show_Separation = true;
};

#endif