#ifndef HEAP_GAUGE_H
#define HEAP_GAUGE_H

#include "./Parts.h"
#include "../../test/RTEPD.h"

#define RAM_MAX 300000

class HeapGauge: public Parts{
    public:
        HeapGauge(uint16_t, uint16_t, CustomSetups*);
        void update();
        void updateWithoutRender();

    private:
        M5EPD_Canvas _canvas_pecentage = M5EPD_Canvas(&M5.EPD);
        char _canvas_hash[RTEPD_SIZE_HASH] = "";
};

#endif