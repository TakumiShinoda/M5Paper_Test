#include "./RTEPD.h"

namespace _RTEPD_PROCESS{
    void canvasRegister(RenderProc*);
    void canvasErase(RenderProc*);
    M5EPD_Canvas* canvasSetFill(RenderProc*);
    void canvasDrawFill(RenderProc*);
    M5EPD_Canvas* canvasSetRect(RenderProc*);
    void canvasDrawRect(RenderProc*);
    M5EPD_Canvas* canvasSetFillRect(RenderProc*);
    void canvasDrawFillRect(RenderProc*);
    M5EPD_Canvas* canvasSetText(RenderProc*);
    void canvasDrawText(RenderProc*);
    M5EPD_Canvas* canvasSetReverse(RenderProc*);
    void canvasDrawReverse(RenderProc*);
    M5EPD_Canvas* canvasSetRawImage(RenderProc*);
    void canvasDrawRawImage(RenderProc*);
    void canvasDraw(RenderProc*);
}

QueueHandle_t RTEPD::Que_RenderProcess = xQueueCreate(20, sizeof(RenderProc*));
static std::vector<HashedCanvas> CanvasRegistry;

void RTEPD::renderProcess(void* params){
    RenderProc* proc;
    BaseType_t Status_Receive;

    while(true){
        Status_Receive = xQueueReceive(RTEPD::Que_RenderProcess, &proc, 0);

        if(Status_Receive != pdPASS) continue;

        Serial.printf("Recieve: %u\n", proc->code);

        if(proc->code == RenderCode::DRAW_FILL_RECT){
            M5.EPD.FillPartGram4bpp(proc->pos.x, proc->pos.y, proc->size.w, proc->size.h, proc->fill_data);
            M5.EPD.UpdateArea(proc->pos.x, proc->pos.y, proc->size.w, proc->size.h, proc->updateMode);
        }else if(proc->code == RenderCode::DRAW_RAW_IMAGE){
            M5.EPD.WritePartGram4bpp(proc->pos.x, proc->pos.y, proc->size.w, proc->size.h, proc->img);
            M5.EPD.UpdateArea(proc->pos.x, proc->pos.y, proc->size.w, proc->size.h, proc->updateMode);
        }else if(proc->code == RenderCode::DRAW_AREA){
            M5.EPD.UpdateArea(proc->pos.x, proc->pos.y, proc->size.w, proc->size.h, proc->updateMode);
        }else if(proc->code == RenderCode::CANVAS_SET_RECT){
            _RTEPD_PROCESS::canvasSetRect(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_RECT){
            _RTEPD_PROCESS::canvasDrawRect(proc);
        }else if(proc->code == RenderCode::CANVAS_SET_FILL_RECT){
            _RTEPD_PROCESS::canvasSetFillRect(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_FILL_RECT){
            _RTEPD_PROCESS::canvasDrawFillRect(proc);
        }else if(proc->code == RenderCode::CANVAS_REGISTER){
            _RTEPD_PROCESS::canvasRegister(proc);
        }else if(proc->code == RenderCode::CANVAS_ERASE){
            _RTEPD_PROCESS::canvasErase(proc);
        }else if(proc->code == RenderCode::CANVAS_SET_FILL){
            _RTEPD_PROCESS::canvasSetFill(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_FILL){
            _RTEPD_PROCESS::canvasDrawFill(proc);
        }else if(proc->code == RenderCode::CANVAS_SET_TEXT){
            _RTEPD_PROCESS::canvasSetText(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_TEXT){
            _RTEPD_PROCESS::canvasDrawText(proc);
        }else if(proc->code == RenderCode::CANVAS_SET_REVERSE){
            _RTEPD_PROCESS::canvasSetReverse(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_REVERSE){
            _RTEPD_PROCESS::canvasDrawReverse(proc);
        }else if(proc->code == RenderCode::CANVAS_SET_RAW_IMAGE){
            _RTEPD_PROCESS::canvasSetRawImage(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW_RAW_IMAGE){
            _RTEPD_PROCESS::canvasDrawRawImage(proc);
        }else if(proc->code == RenderCode::CANVAS_DRAW){
            _RTEPD_PROCESS::canvasDraw(proc);
        }
        
        proc->flag_processed = true;
        delete proc;
        // vTaskDelay(1 * portTICK_PERIOD_MS);
    }
}

M5EPD_Canvas* RTEPD::getCanvas(char* hash){
    for(uint8_t i = 0; i < CanvasRegistry.size(); i++){
        if(strcmp(CanvasRegistry[i].hash, hash) != 0) continue;

        return CanvasRegistry[i].canvas;
    }

    return nullptr;
}

void _RTEPD_PROCESS::canvasRegister(RenderProc* proc){
    M5EPD_Canvas* NewCanvas;
    char* NewHash;
    uint8_t LoopCnt_HashCheck;
    uint8_t Size_CanvasRegister = CanvasRegistry.size();

    if(proc->canvasParams.hash == nullptr) RTEPD_ERR_MES();
    if(strlen(proc->canvasParams.hash) != RTEPD_LEN_HASH) RTEPD_ERR_MES();

    for(LoopCnt_HashCheck = 0; LoopCnt_HashCheck < Size_CanvasRegister; LoopCnt_HashCheck++){
        if(strcmp(CanvasRegistry[LoopCnt_HashCheck].hash, proc->canvasParams.hash) == 0) break;
    }
    if(LoopCnt_HashCheck != Size_CanvasRegister) RTEPD_ERR_MES();

    NewHash = (char*)malloc((size_t)(RTEPD_LEN_HASH + 1));
    NewCanvas = new M5EPD_Canvas(&M5.EPD);
    strcpy(NewHash, proc->canvasParams.hash);

    //Serial.printf("Register: %s -> %p\n", NewHash, NewCanvas);

    if(Size_CanvasRegister > 256) RTEPD_ERR_MES();
    if(NewCanvas == nullptr || NewHash == nullptr) RTEPD_ERR_MES();

    NewCanvas->createCanvas(proc->size.w, proc->size.h);
    CanvasRegistry.push_back((HashedCanvas){NewHash, NewCanvas});
}

void _RTEPD_PROCESS::canvasErase(RenderProc* proc){
    uint8_t Size_CanvasRegistry = CanvasRegistry.size();
    
    if(proc->canvasParams.hash == nullptr) RTEPD_ERR_MES();

    for(uint8_t i = 0; i < Size_CanvasRegistry; i++){
        if(strcmp(CanvasRegistry[i].hash, proc->canvasParams.hash) == 0){
            delete CanvasRegistry[i].hash;
            delete CanvasRegistry[i].canvas;
            CanvasRegistry.erase(CanvasRegistry.begin() + i);
        }
        Serial.println("Loop");
    }
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetFill(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    if(proc->canvasParams.hash == nullptr) return nullptr;

    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return nullptr;

    Canvas->fillCanvas(proc->canvasParams.color);

    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawFill(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    Canvas = _RTEPD_PROCESS::canvasSetRect(proc);

    if(Canvas == nullptr) return;

    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetRect(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    //Serial.printf("Hash: %s\n", proc->canvasParams.hash);

    if(proc->canvasParams.hash == nullptr) return nullptr;

    //Serial.printf("Getting\n");
    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return nullptr;

    //Serial.println("RECT");
    Canvas->drawRect(proc->canvasParams.innerPos.x, proc->canvasParams.innerPos.y, proc->size.w, proc->size.h, proc->canvasParams.color);

    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawRect(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    Canvas = _RTEPD_PROCESS::canvasSetRect(proc);

    if(Canvas == nullptr) return;

    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetFillRect(RenderProc* proc){
     M5EPD_Canvas* Canvas;

    Serial.println("SFill1");
    if(proc->canvasParams.hash == nullptr) return nullptr;

    Serial.println("SFill2");
    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    Serial.println("SFill3");
    if(Canvas == nullptr) return nullptr;

    Serial.println("SFill4");
    Canvas->fillRect(proc->canvasParams.innerPos.x, proc->canvasParams.innerPos.y, proc->size.w, proc->size.h, proc->canvasParams.color);

    Serial.println("SFill5");
    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawFillRect(RenderProc* proc){
    Serial.println("Fill1");
    M5EPD_Canvas* Canvas = _RTEPD_PROCESS::canvasSetFillRect(proc);

    Serial.println("Fill2");
    if(Canvas == nullptr) return;

    Serial.println("Fill3");
    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
    Serial.println("Fill4");
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetText(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    if(proc->canvasParams.hash == nullptr) return nullptr;

    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return nullptr;

    Canvas->setTextSize(proc->canvasParams.textSize);
    Canvas->drawString(proc->canvasParams.text, proc->canvasParams.innerPos.x, proc->canvasParams.innerPos.y);

    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawText(RenderProc* proc){
    M5EPD_Canvas* Canvas = _RTEPD_PROCESS::canvasSetText(proc);

    if(Canvas == nullptr) return;

    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetReverse(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    if(proc->canvasParams.hash == nullptr) return nullptr;

    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return nullptr;

    Canvas->ReverseColor();

    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawReverse(RenderProc* proc){
    M5EPD_Canvas* Canvas = _RTEPD_PROCESS::canvasSetReverse(proc);

    if(Canvas == nullptr) return;

    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}

M5EPD_Canvas* _RTEPD_PROCESS::canvasSetRawImage(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    if(proc->canvasParams.hash == nullptr) return nullptr;
    if(proc->img == nullptr) return nullptr;

    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return nullptr;

    Canvas->pushImage(proc->canvasParams.innerPos.x, proc->canvasParams.innerPos.y, proc->size.w, proc->size.h, proc->img);

    return Canvas;
}

void _RTEPD_PROCESS::canvasDrawRawImage(RenderProc* proc){
    M5EPD_Canvas* Canvas = _RTEPD_PROCESS::canvasSetRawImage(proc);

    if(Canvas == nullptr) RTEPD_ERR_MES();

    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}

void _RTEPD_PROCESS::canvasDraw(RenderProc* proc){
    M5EPD_Canvas* Canvas;

    if(proc->canvasParams.hash == nullptr) return;

    Canvas = RTEPD::getCanvas(proc->canvasParams.hash);

    if(Canvas == nullptr) return;

    //Serial.printf("Draw: %s -> %p\n", proc->canvasParams.hash, Canvas);
    Canvas->pushCanvas(proc->pos.x, proc->pos.y, proc->updateMode);
}