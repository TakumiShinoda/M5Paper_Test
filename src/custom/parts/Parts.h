#ifndef PARTS_H
#define PARTS_H

#include <Arduino.h>

#include "../setups.h"

typedef uint8_t PriorityNum;

class Parts{
    public:
        Parts(uint16_t, uint16_t, CustomSetups*, PriorityNum = 0);
        virtual void init();
        virtual void update();
        virtual void updateWithoutRender();
        void setPos(uint16_t, uint16_t);
        void setDisable(bool);
        static void updates(std::vector<Parts*>);
        static void setDisables(std::vector<Parts*>);
        static void inits(std::vector<Parts*>);
        static bool createHash(char*, uint8_t);
        bool _flag_debug = false;

    protected:
        bool isReadyToUpdate();
        void setLastUpdate();
        void debugPrint(String);
        bool _flag_stop_render = false;
        bool _flag_disable = false;
        uint32_t _millis_last_update = 0;
        uint32_t _millis_update_span = 100;
        uint16_t _x = 0;
        uint16_t _y = 0;
        uint16_t _w = 0;
        uint16_t _h = 0;
        CustomSetups* _setup;
        PriorityNum _priority;
};

#endif