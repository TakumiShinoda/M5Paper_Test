#ifndef RTRTC_H
#define RTRTC_H

#include <M5EPD.h>

#include "./Mutexes.h"

namespace RTRTC{
    extern rtc_time_t Now;
    void touchProcess(void*);
}

#endif