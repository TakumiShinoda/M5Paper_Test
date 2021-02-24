#include "./global.h"

int32_t Global::heapStart = 0;

void Global::setupHeapStart(){
    uint8_t* Mem = (uint8_t*)malloc(sizeof(uint8_t));
    uint8_t* Mem2 = (uint8_t*)malloc(sizeof(uint8_t));

    if(Mem == nullptr) esp_restart();

    Global::heapStart = (int32_t)Mem;
    Serial.printf("BBBBBBBB: %p\n", "hogehoge");
    Serial.printf("CCCCCCCC: %u, %p\n", (uint32_t)&_bss_end, &_bss_end);
    Serial.printf("AAAAAAAA: %d, %p\n", Global::heapStart, Global::heapStart);

    Serial.printf("DDDDDDDD: %d, %p\n", Mem2, Mem2);
    free(Mem);
}