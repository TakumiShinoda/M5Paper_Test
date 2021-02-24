#ifndef SETUPS_H
#define SETUPS_H

#include <M5EPD.h>
#include <WiFi.h>
#include <SD.h>
#include <time.h>

const char SSID[] = "junichi_saikyo24";
const char PASS[] = "godgodx4";

class CustomSetups{
    public:
        static void Wifi();
        static void MMC_SD();
        void TP();
        void EPD();
        void RTC();
        bool isAvailableEPD();
        bool isAvailableTP();
        bool isAvailableRTC();
        void update();
        bool getAvailable_TP();
        bool getIsFingerUp_TP();
        static uint64_t getEpoch(rtc_date_t*, rtc_time_t*);

    private:
        bool _IsAvailable_EPD = false;
        bool _IsAvailable_TP = false;
        bool _IsAvailable_RTC = false;
        bool _flag_tp_available = false;
        bool _flag_tp_fingerup = true;
};

#endif