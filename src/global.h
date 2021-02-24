#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

extern char _bss_end;

namespace Global{
    extern int32_t heapStart;

    void setupHeapStart();
}

#endif