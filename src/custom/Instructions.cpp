#include "./Instructions.h"

void Instructions::renderRect(InstructionParams::RenderRect* params){
    char Hash[RTEPD_SIZE_HASH];

    Parts::createHash(Hash, RTEPD_SIZE_HASH);
    if(RTEPD_API::canvasRegister(Hash, params->w, params->h, 0) == nullptr) return; 
    RTEPD_API::canvasDrawRect(Hash, params->x, params->y + POS_APP_AREA_Y, 0, 0, params->w, params->h, params->data, UPDATE_MODE_DU4, 0);
    RTEPD_API::canvasErase(Hash, portMAX_DELAY);
}

void Instructions::renderFillRect(InstructionParams::RenderFillRect* params){
    char Hash[RTEPD_SIZE_HASH];

    Parts::createHash(Hash, RTEPD_SIZE_HASH);
    if(RTEPD_API::canvasRegister(Hash, params->w, params->h, 0) == nullptr) return;
    RTEPD_API::canvasDrawFillRect(Hash, params->x, params->y + POS_APP_AREA_Y, 0, 0, params->w, params->h, params->data, UPDATE_MODE_DU4, 0);
    RTEPD_API::canvasErase(Hash, portMAX_DELAY);
}

void Instructions::jump(InstructionParams::Jump* params, InstructionParams::_Output* output){
    *(output->index_instruction) = params->index_new;
    *(output-> flag_jumped) = true;
}