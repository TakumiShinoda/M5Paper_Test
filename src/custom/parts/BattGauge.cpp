#include "./BattGauge.h"

BattGauge::BattGauge(uint16_t x, uint16_t y, CustomSetups* setup, PriorityNum priority): Parts(x, y, setup, priority){
    this->_w = WIDTH_PARTS_BATT_GAUGE_FRAME;
    this->_h = HEIGHT_PARTS_BATT_GAUGE_FRAME;
    this->_millis_update_span = 1000;
    
    Parts::createHash(this->_canvas_hash, RTEPD_SIZE_HASH);
    RTEPD_API::canvasRegister(this->_canvas_hash, 22, 13);
}

void BattGauge::init(){
    Parts::init();
    this->_is_override = false;
    this->_is_rendered_gauge_frame = false;
}

void BattGauge::update(){
    // Serial.println("Batt");
    if(!this->_setup->isAvailableEPD()) return;
    if(!this->isReadyToUpdate()) return;

    this->updateWithoutRender();

    RTEPD_API::canvasDraw(this->_canvas_hash, this->_x + 1, this->_y + 3, UPDATE_MODE_DU4);
    RTEPD_API::drawArea(this->_x, this->_y, WIDTH_PARTS_BATT_GAUGE_FRAME, HEIGHT_PARTS_BATT_GAUGE_FRAME, UPDATE_MODE_DU4);

    this->setLastUpdate();
    this->debugPrint("Update: BattGauge");
}

void BattGauge::updateWithoutRender(){
    uint8_t Val_GaugeLevel;
    uint32_t Val_Voltage;

    if(this->_is_override){
        Val_GaugeLevel = (uint8_t)map(this->_val_gauge_percent, 0, 100, 0, 22);
    }else{
        Val_Voltage = M5.getBatteryVoltage();

        if(Val_Voltage > LIMIT_UPPER_BATT_VOLTAGE) Val_Voltage = LIMIT_UPPER_BATT_VOLTAGE;
        if(Val_Voltage < LIMIT_LOWER_BATT_VOLTAGE) Val_Voltage = LIMIT_LOWER_BATT_VOLTAGE; 

        Val_GaugeLevel = (uint8_t)map(Val_Voltage, LIMIT_LOWER_BATT_VOLTAGE, LIMIT_UPPER_BATT_VOLTAGE, 0, 22);
    }

    if(!this->_is_rendered_gauge_frame){
        RTEPD_API::drawRawImage(this->_x, this->_y, WIDTH_PARTS_BATT_GAUGE_FRAME, HEIGHT_PARTS_BATT_GAUGE_FRAME, (uint8_t*)PARTS_BATT_GAUGE_FRAME, UPDATE_MODE_DU4);
        this->_is_rendered_gauge_frame = true;
    }

    RTEPD_API::canvasSetFill(this->_canvas_hash, 0x0000);
    RTEPD_API::canvasSetFillRect(this->_canvas_hash, 0, 0, Val_GaugeLevel, 16, 0xFFFF);
}

void BattGauge::setOverrideValue(uint8_t Val_Gauge_Percent){
    if(Val_Gauge_Percent > 100) Val_Gauge_Percent = 100;

    this->_val_gauge_percent = Val_Gauge_Percent;
}

void BattGauge::setOverrideMode(bool Is_Override){
    this->_is_override = Is_Override;
}