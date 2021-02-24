#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "./setups.h"
#include "./AppArea.h"
#include "../test/RTEPD.h"

namespace InstructionParams{
    typedef struct{
        uint32_t* index_instruction;
        bool* flag_jumped;
    } _Output;
    
    typedef struct{
        uint16_t x;
        uint16_t y;
        uint16_t w;
        uint16_t h;
        uint16_t data;
    } RenderRect;

    typedef struct{
        uint16_t x;
        uint16_t y;
        uint16_t w;
        uint16_t h;
        uint16_t data;
    } RenderFillRect;

    typedef struct{
        uint32_t index_new;
    } Jump;
    
};

namespace Instructions{
    void renderRect(InstructionParams::RenderRect*);
    void renderFillRect(InstructionParams::RenderFillRect*);
    void jump(InstructionParams::Jump*, InstructionParams::_Output*);
};

#endif