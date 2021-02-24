#include "./RTEPD.h"

RenderProc* RTEPD_API::drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;

    Proc->code = RenderCode::DRAW_FILL_RECT;
    Proc->pos = {x, y};
    Proc->size = {w, h};
    Proc->fill_data = color;
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::drawRawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* img, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;

    Proc->code = RenderCode::DRAW_RAW_IMAGE;
    Proc->pos = {x, y};
    Proc->size = {w, h};
    Proc->img = img;
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;

    Proc->code = RenderCode::DRAW_AREA;
    Proc->pos = {x, y};
    Proc->size = {w, h};
    Proc->updateMode = mode;
    
    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasRegister(const char* hash, uint16_t w, uint16_t h){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_REGISTER;
    Proc->canvasParams.hash = Hash;
    Proc->size = {w, h};

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasErase(const char* hash){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_ERASE;
    Proc->canvasParams.hash = Hash;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasSetFill(const char* hash, uint16_t color){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_FILL;
    Proc->canvasParams.hash = Hash;
    Proc->canvasParams.color = color;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawFill(const char* hash, uint16_t x, uint16_t y, uint16_t color, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_FILL;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->canvasParams.color = color;
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasSetRect(const char* hash, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint16_t color){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_RECT;
    Proc->canvasParams.hash = Hash;
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->canvasParams.color = color;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawRect(const char* hash, uint16_t x, uint16_t y, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint16_t color, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_RECT;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->updateMode = mode;
    Proc->canvasParams.color = color;

    RTEPD_API::addToQue(Proc);

    return Proc;    
}

RenderProc* RTEPD_API::canvasSetFillRect(const char* hash, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint16_t color){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_FILL_RECT;
    Proc->canvasParams.hash = Hash;
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->canvasParams.color = color;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawFillRect(const char* hash, uint16_t x, uint16_t y, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint16_t color, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_FILL_RECT;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->canvasParams.color = color;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasSetText(const char* hash, uint16_t in_x, uint16_t in_y, uint8_t textSize, char* text){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_TEXT;
    Proc->canvasParams.hash = Hash;
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->canvasParams.textSize = textSize;
    Proc->canvasParams.text = text;
    
    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawText(const char* hash, uint16_t x, uint16_t y, uint16_t in_x, uint16_t in_y, uint8_t textSize, char* text, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_TEXT;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->canvasParams.textSize = textSize;
    Proc->canvasParams.text = text;
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasSetReverse(const char* hash){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_REVERSE;
    Proc->canvasParams.hash = Hash;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawReverse(const char* hash, uint16_t x, uint16_t y, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_REVERSE;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasSetRawImage(const char* hash, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint8_t* img){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_SET_RAW_IMAGE;
    Proc->canvasParams.hash = Hash;
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->img = img;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDrawRawImage(const char* hash, uint16_t x, uint16_t y, uint16_t in_x, uint16_t in_y, uint16_t w, uint16_t h, uint8_t* img, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW_RAW_IMAGE;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->canvasParams.innerPos = {in_x, in_y};
    Proc->size = {w, h};
    Proc->img = img;
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

RenderProc* RTEPD_API::canvasDraw(const char* hash, uint16_t x, uint16_t y, m5epd_update_mode_t mode){
    RenderProc* Proc = new RenderProc;
    char* Hash = (char*)malloc(sizeof(char) + strlen(hash));

    strcpy(Hash, hash);
    Proc->code = RenderCode::CANVAS_DRAW;
    Proc->canvasParams.hash = Hash;
    Proc->pos = {x, y};
    Proc->updateMode = mode;

    RTEPD_API::addToQue(Proc);

    return Proc;
}

void RTEPD_API::addToQue(RenderProc* proc){
    xQueueSend(RTEPD::Que_RenderProcess, &proc, portMAX_DELAY);
}

void RTEPD_API::gabageCollectionProccess(void* params){
    while(true){
        for(uint32_t i = 0; i < RTEPD_API::gabageCollection.size(); i++){
            if(RTEPD_API::gabageCollection[i]->flag_processed){
                free(RTEPD_API::gabageCollection[i]);
            }
        }

        vTaskDelay(1);
    }
}