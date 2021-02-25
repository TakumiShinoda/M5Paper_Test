#include "./global.h"

int32_t Global::heapStart = 0;

void Global::setupHeapStart(){
    uint8_t* Mem = (uint8_t*)malloc(sizeof(uint8_t));
    uint8_t* Mem2 = (uint8_t*)malloc(sizeof(uint8_t));

    if(Mem == nullptr) esp_restart();

    Global::heapStart = (int32_t)Mem;
    free(Mem);
}