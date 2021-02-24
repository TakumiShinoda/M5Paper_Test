#ifndef RTTP_H
#define RTTP_H

#include <M5EPD.h>

#include "../types.h"
#include "./Mutexes.h"

namespace RTTP{
    extern std::vector<tp_finger_t> Fingers;
    extern Vector2 Pos;
    extern bool IsTouch;
    void _touchProcess();
}

void touchProcess(void*);

#endif