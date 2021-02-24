#ifndef CUSTOM_H
#define CUSTOM_H

#include <M5EPD.h>
#include <WiFi.h>
#include <SD.h>
#include <ServerObject.h>
#include "epdgui/epdgui.h"

#include "./setups.h"
#include "./parts/BattGauge.h"
#include "./parts/Bar.h"
#include "./parts/DigitalClock.h"
#include "./parts/RectButton.h"
#include "./parts/HeapGauge.h"
#include "./AppArea.h"
#include "./Instructions.h"

#define LEVER_FALSE 1
#define LEVER_TRUE 0

#define MODE_MAIN_SYSTEM_FACTORY 0
#define MODE_MAIN_SYSTEM_CUSTOM 1

#define TIME_SPAN_EPD_CLEAR 30

#define LEN_STATUS_BAR 4

void View_TouchTest();
void View_CustomSystem(CustomSetups*);

#endif