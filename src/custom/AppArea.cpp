#include "./AppArea.h"

AppArea::AppArea(std::vector<Parts*> statusBar, CustomSetups* setup){  
    this->_parts_status_bar = statusBar;
    this->_setup = setup;
}

void AppArea::registerApp(AppInfo info){
    this->_app_infos.push_back(info);
}

void AppArea::launchApp(const uint8_t* appCode){
    RTEPD_API::drawFillRect(0, POS_APP_AREA_Y, 540, 960 - POS_APP_AREA_Y, 0xFFFF, UPDATE_MODE_GC16);
    
    this->_info_main_proccess = nullptr;

    for(uint64_t i = 0; i < this->_app_infos.size(); i++){
        if(strcmp((char*)appCode, (char*)this->_app_infos[i].str_appCode) != 0) continue;

        this->_info_main_proccess = &this->_app_infos[i];
    }

    if(this->_info_main_proccess == nullptr){
        this->showLaunchFailed();
        return;
    }

    this->_info_main_proccess->index_instruction = 0;

    bool NowTouch = false;
    tp_finger_t NowTouchFinger;
    Vector2 Pos_SwipeStart = {0, 0};
    bool WasTouch = false;
    bool OnTouch = false;
    bool Released = false;
    bool IsSwiping = false;
    bool IsSwiped = false;

    while(true){
        Parts::updates(this->_parts_status_bar);
        uint8_t Code_Exception;

        NowTouch = RTTP::IsTouch;
        NowTouchFinger = RTTP::Fingers[0];

        if(NowTouch && !WasTouch){
            OnTouch = true;
        }else{
            OnTouch = false;
        }
        
        if(!NowTouch && WasTouch){
            Released = true;
        }else{
            Released = false;
        }

        WasTouch = NowTouch;

        if(OnTouch){
            Pos_SwipeStart.x = NowTouchFinger.x;
            Pos_SwipeStart.y = NowTouchFinger.y;
            IsSwiping = true;
            Serial.println("OnTouch");
        }else if(Released){
            Serial.println("Released");
        }

        if(IsSwiping){
            Serial.printf("%d\n", abs(NowTouchFinger.y - Pos_SwipeStart.y));

            if(abs(NowTouchFinger.y - Pos_SwipeStart.y) > 100 && Released){
                IsSwiped = true;
                IsSwiping = false;
            }else{
                IsSwiped = false;
            }

            if(
                IsSwiped &&
                (Pos_SwipeStart.x >= 0) && (Pos_SwipeStart.x <= 180) &&
                (Pos_SwipeStart.y >= 940 - 50) && (Pos_SwipeStart.y <= 940)
            ){
                Serial.println("Swiped");
                while(true){
                    RenderProc* Retp;
                    BaseType_t Result;
                    
                    Result = xQueueReceive(RTEPD::Que_RenderProcess, &Retp, 10 / portTICK_PERIOD_MS);
                    
                    if(uxQueueMessagesWaiting(RTEPD::Que_RenderProcess) == 0) break;
                    if(Retp == nullptr) continue;

                    delete Retp;
                }
                
                break;
            }
        }

        if(this->_info_main_proccess->flag_buildin){
            if(this->_info_main_proccess == nullptr){
                this->showLaunchFailed();
                return;
            }
            
            if(this->_info_main_proccess->index_instruction >= this->_info_main_proccess->instructions.size()){
                this->showException(255);
                return;
            }

            Code_Exception = processor(&this->_info_main_proccess->instructions[this->_info_main_proccess->index_instruction], &(this->_info_main_proccess->index_instruction));

            if(Code_Exception != 0){
                this->showException(Code_Exception);
                return;
            }
        }else{
            return;
        }
    }

    RTEPD_API::drawFillRect(0, POS_APP_AREA_Y, 540, 960 - POS_APP_AREA_Y, 0xFFFF, UPDATE_MODE_GC16);
}

void AppArea::showLaunchFailed(){
    RTEPD_API::canvasRegister("0123456789", 540, 50);
    RTEPD_API::canvasDrawText("0123465789", (540 / 2) - 150, (960 / 2), 0, 0, 3, "Failed to launch", UPDATE_MODE_DU4);
    delay(2000);
    RTEPD_API::canvasDrawFill("0123465789", 0, 0, 0x0000, UPDATE_MODE_A2);
}

void AppArea::showException(uint8_t code){
    char Mes[21] = "";

    sprintf(Mes, "Exception %hhu", code);
    RTEPD_API::canvasRegister("1234567890", 540, 50);
    RTEPD_API::canvasDrawText("1234567890", (540 / 2) - 150, (960 / 2), 0, 0, 3, Mes, UPDATE_MODE_GL16);
    delay(2000);
    RTEPD_API::drawFillRect(0, 0, 540, 960, 0xFFFF, UPDATE_MODE_GL16);
    RTEPD_API::drawFillRect(0, POS_APP_AREA_Y, 540, 960 - POS_APP_AREA_Y, 0xFFFF, UPDATE_MODE_GC16);
}

uint8_t AppArea::processor(Instruction* instruction, uint32_t* index_instruction){
    bool Flag_Jumped = false;
    InstructionParams::_Output Outputs = {
        index_instruction,
        &Flag_Jumped
    };

    if(instruction->code == InstructionCode::RENDER_RECT){
        Instructions::renderRect((InstructionParams::RenderRect*)instruction->args);
    }else if(instruction->code == InstructionCode::RENDER_FILL_RECT){
        Instructions::renderFillRect((InstructionParams::RenderFillRect*)instruction->args);
    }else if(instruction->code == InstructionCode::JUMP){
        Instructions::jump((InstructionParams::Jump*)instruction->args, &Outputs);
    }else{
        return 1;
    }

    if(!Flag_Jumped) *(index_instruction) += 1;
    
    return 0;
}