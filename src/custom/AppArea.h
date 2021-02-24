#ifndef APP_AREA_H
#define APP_AREA_H

#include "./setups.h"
#include "./parts/Parts.h"
#include "./Instructions.h"

#define POS_APP_AREA_Y 53

enum class InstructionCode: uint32_t{
    RENDER_RECT,
    RENDER_FILL_RECT,
    JUMP
};

typedef struct{
    InstructionCode code;
    void* args;
} Instruction;

typedef struct{
    bool flag_buildin = false;
    std::vector<Instruction> instructions;
    uint8_t* str_appCode;
    uint32_t index_instruction = 0;
    uint8_t mode_screen;
} AppInfo;

class AppArea{
    public:
        AppArea(std::vector<Parts*>, CustomSetups*);
        void registerApp(AppInfo);
        void launchApp(const uint8_t*);

    private:
        void showLaunchFailed();
        void showException(uint8_t);
        uint8_t processor(Instruction*, uint32_t*);
        CustomSetups* _setup;
        AppInfo* _info_main_proccess;
        std::vector<Parts*> _parts_status_bar;
        std::vector<AppInfo> _app_infos;
};

#endif