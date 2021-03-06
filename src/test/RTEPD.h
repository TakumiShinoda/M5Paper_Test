#ifndef RTEPD_H
#define RTEPD_H

#include <M5EPD.h>
#include <stdlib.h>

#include "../types.h"
#include "./global.h"

#define RTEPD_LEN_HASH 10
#define RTEPD_SIZE_HASH (RTEPD_LEN_HASH + 1)

#define RTEPD_ERR_MES() {\
    do{\
        Serial.printf("[%s %u] Error.\n", __func__, __LINE__);\
        return;\
    }while(false);\
}

enum class RenderCode{
    NONE,
    DRAW_FILL_RECT,
    DRAW_RAW_IMAGE,
    DRAW_AREA,
    CANVAS_REGISTER,
    CANVAS_ERASE,
    CANVAS_IGNORE,
    CANVAS_SET_FILL,
    CANVAS_DRAW_FILL,
    CANVAS_SET_RECT,
    CANVAS_DRAW_RECT,
    CANVAS_SET_FILL_RECT,
    CANVAS_DRAW_FILL_RECT,
    CANVAS_SET_TEXT,
    CANVAS_DRAW_TEXT,
    CANVAS_SET_REVERSE,
    CANVAS_DRAW_REVERSE,
    CANVAS_SET_RAW_IMAGE,
    CANVAS_DRAW_RAW_IMAGE,
    CANVAS_DRAW
};

typedef struct _CanvasParams{
    char* text = nullptr;
    Vector2 innerPos = {0, 0};
    char* hash = nullptr;
    uint8_t textSize = 1;
    uint16_t color = 0xFFFF;

    ~_CanvasParams(){
        if((uint32_t)text <= (uint32_t)Global::heapStart && (uint32_t)text >= (uint32_t)Global::heapStart - (uint32_t)heap_caps_get_free_size(MALLOC_CAP_INTERNAL)){
            delete text;
        }
        delete hash;
    }
} CanvasParams;

typedef struct _RenderProc{
    bool flag_processed = false;
    RenderCode code = RenderCode::NONE;
    Vector2 pos = {0, 0};
    Size size = {0, 0};
    uint16_t fill_data = 0x0000;
    uint8_t* img = nullptr;
    CanvasParams canvasParams;
    m5epd_update_mode_t updateMode = UPDATE_MODE_GC16;

    ~_RenderProc(){
        if((uint32_t)img <= (uint32_t)Global::heapStart && (uint32_t)img >= (uint32_t)Global::heapStart - (uint32_t)heap_caps_get_free_size(MALLOC_CAP_INTERNAL)){
            delete img;
        }
    }
} RenderProc;

typedef struct _HashedCanvas{
    char* hash;
    M5EPD_Canvas* canvas;
    bool isIgnore = false;
} HashedCanvas;

namespace RTEPD{
    extern QueueHandle_t Que_RenderProcess;

    void renderProcess(void*);
    HashedCanvas* getHashedCanvas(char*);
    M5EPD_Canvas* getCanvas(char*, bool);
}

namespace RTEPD_API{
    RenderProc* drawFillRect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, m5epd_update_mode_t);
    RenderProc* drawRawImage(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*, m5epd_update_mode_t);
    RenderProc* drawArea(uint16_t, uint16_t, uint16_t, uint16_t, m5epd_update_mode_t);

    RenderProc* canvasRegister(const char*, uint16_t, uint16_t);
    RenderProc* canvasErase(const char*);
    RenderProc* canvasIgnore(const char*);
    RenderProc* canvasSetFill(const char*, uint16_t);
    RenderProc* canvasDrawFill(const char*, uint16_t, uint16_t, uint16_t, m5epd_update_mode_t);
    RenderProc* canvasSetRect(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
    RenderProc* canvasDrawRect(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, m5epd_update_mode_t);
    RenderProc* canvasSetFillRect(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
    RenderProc* canvasDrawFillRect(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, m5epd_update_mode_t);
    RenderProc* canvasSetText(const char*, uint16_t, uint16_t, uint8_t, char*);
    RenderProc* canvasDrawText(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t, char*, m5epd_update_mode_t);
    RenderProc* canvasSetReverse(const char*);
    RenderProc* canvasDrawReverse(const char*, uint16_t, uint16_t, m5epd_update_mode_t);
    RenderProc* canvasSetRawImage(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*);
    RenderProc* canvasDrawRawImage(const char*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t*, m5epd_update_mode_t);
    RenderProc* canvasDraw(const char*, uint16_t, uint16_t, m5epd_update_mode_t);

    void addToQue(RenderProc*);
    void gabageCollectionProccess(void* params);

    static std::vector<RenderProc*> gabageCollection;
}

#endif