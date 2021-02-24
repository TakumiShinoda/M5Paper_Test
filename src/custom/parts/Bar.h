#ifndef BAR_H
#define BAR_H

#include "./Parts.h"
#include "../../test/RTEPD.h"

class Bar: public Parts{
    public: 
        Bar(uint16_t, uint16_t, CustomSetups*, PriorityNum = 0);
        void update();
        void updateWithoutRender();

    private:
        bool _flag_rendered = false;
        char _canvas_hash[RTEPD_SIZE_HASH];
};

#endif