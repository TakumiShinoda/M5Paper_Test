#include <M5EPD.h>
#include "epdgui/epdgui.h"
#include "frame/frame.h"
#include "systeminit.h"

#include "./global.h"
#include "./custom/custom.h"
#include "./custom/setups.h"

#include "test/RTEPD.h"
#include "test/RTTP.h"
#include "test/RTRTC.h"

void setupTP();
void setupEPD();

CustomSetups Setup;
M5EPD_Canvas canvas(&M5.EPD);
uint8_t Mode_MainSystem = MODE_MAIN_SYSTEM_FACTORY;

char hoge[11] = "hogehogeaa";
char hoge2[] = "hoge";
RenderProc Aiu;
RenderProc AAA;
RenderProc BBB;
RenderProc* _Aiu = &Aiu;
RenderProc* _AAA = &AAA;
RenderProc* _BBB = &BBB;

QueueHandle_t testQ = xQueueCreate(10, sizeof(RenderProc*));

void setup(){
    Serial.begin(115200);
    // RenderProc* Proc_Register = RTEPD_API::canvasRegister("0123456789", 92, 92);
    // RenderProc* Proc_Rect = ;
    // RenderProc* Proc_Image = RTEPD_API::canvasDrawRawImage("0123456789", 100, 100, 0, 0, 92, 92, (uint8_t*)ImageResource_main_icon_setting_92x92, UPDATE_MODE_DU4);

    pinMode(M5EPD_MAIN_PWR_PIN, OUTPUT);
    pinMode(M5EPD_KEY_LEFT_PIN, INPUT);
    M5.enableMainPower();
    Global::setupHeapStart();

    if(digitalRead(M5EPD_KEY_LEFT_PIN) == LEVER_FALSE){
        Mode_MainSystem = MODE_MAIN_SYSTEM_CUSTOM;
        Setup.EPD();
        Setup.TP();
        Setup.RTC();
        // canvas.createCanvas(540, 960);
        // canvas.setTextSize(6);
        Serial.printf("size: %u\n", sizeof(RenderProc));
    }else{
        SysInit_Start();
    }

    // Proc_Register->code = RenderCode::CANVAS_REGISTER;
    // Proc_Register->canvasParams.hash = "0123456789";
    // Proc_Register->size = {100, 100};

    // Proc_Rect->code = RenderCode::CANVAS_DRAW_RECT;
    // Proc_Rect->canvasParams.hash = "0123456789";
    // Proc_Rect->pos = {100, 100};
    // Proc_Rect->size = {100, 100};
    // Proc_Rect->updateMode = UPDATE_MODE_DU4;

    // Proc_Image->code = RenderCode::CANVAS_DRAW_RAW_IMAGE;
    // Proc_Image->canvasParams.hash = "0123456789";
    // Proc_Image->pos = {100, 100};
    // Proc_Image->size = {92, 92};
    // Proc_Image->img = (uint8_t*)ImageResource_main_icon_setting_92x92;

    if(Mode_MainSystem == MODE_MAIN_SYSTEM_CUSTOM){
        xTaskCreatePinnedToCore(RTEPD::renderProcess, "Render", 8192, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(touchProcess, "Touch", 4096, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(RTRTC::touchProcess, "RTC", 4096, NULL, 1, NULL, 1);
        // xTaskCreatePinnedToCore(RTEPD_API::gabageCollectionProccess, "RTEPD_API_GABAGE_COLLECTION", 4096, NULL, 1, NULL, 0);
    }
}

bool wasTouch = false;

void loop(){
    RenderProc* Buff_Proc;

    #if 1
    if(Mode_MainSystem == MODE_MAIN_SYSTEM_CUSTOM){
        Serial.println("Custom");
        //View_TouchTest();
        View_CustomSystem(&Setup);
    }else if(Mode_MainSystem == MODE_MAIN_SYSTEM_FACTORY){
        Serial.println("Factory");
        EPDGUI_MainLoop();
    }else{
        Serial.println("Failed");
        M5.EPD.Clear();
        delay(TIME_SPAN_EPD_CLEAR);
        M5.EPD.WritePartGram4bpp(32 * 4, 0, 92, 92, ImageResource_home_icon_conditioner_on_92x92);
        return;
    }

    if(RTTP::IsTouch){
        Serial.printf("touch: %u, %u\n", RTTP::Fingers[0].x, RTTP::Fingers[0].y);
        Serial.printf("count: %u\n\n", RTTP::Fingers.size());
    }else Serial.println("not");

    Serial.printf("Proc: %u\n", Aiu.flag_processed ? 1 : 0);
    #else
    // if(RTTP::IsTouch && !wasTouch){
    //     Serial.println("touch");
    //     // RTEPD_API::canvasDrawReverse("0123456789", 100, 100, UPDATE_MODE_DU4);
    //     RTEPD_API::canvasSetReverse("0123456789");
    //     RTEPD_API::canvasDraw("0123456789", 100, 100, UPDATE_MODE_DU4);
    // }else if(!RTTP::IsTouch && wasTouch){
    //     Serial.println("released");
    //     // RTEPD_API::canvasDrawReverse("0123456789", 100, 100, UPDATE_MODE_DU4);
    //     RTEPD_API::canvasSetReverse("0123456789");
    //     RTEPD_API::canvasDraw("0123456789", 100, 100, UPDATE_MODE_DU4);
    // }

    // wasTouch = RTTP::IsTouch;
    
    // RenderProc* newP = new RenderProc;
    
    // newP->canvasParams.hash = "123456789";

    // xQueueSend(RTEPD::Que_RenderProcess, &newP, portMAX_DELAY);
    char* Str_Now = (char*)malloc(sizeof(char) * 11);

    sprintf(Str_Now, "%s", "1234567890");
    RTEPD_API::canvasRegister(Str_Now, 100, 100);
    Serial.printf("Heap: %u\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    #endif

    delay(50);
}