#include "./HeapGauge.h"

HeapGauge::HeapGauge(uint16_t x, uint16_t y, CustomSetups* setup): Parts(x, y, setup){
    Parts::createHash(this->_canvas_hash, RTEPD_SIZE_HASH);
    RTEPD_API::canvasRegister(this->_canvas_hash, 76, 20);

    this->_millis_update_span = 3000;
}

void HeapGauge::update(){
    Serial.println("Heap");
    if(!this->_setup->isAvailableEPD()) return;
    if(!this->isReadyToUpdate()) return;

    this->updateWithoutRender();
    this->setLastUpdate();
    RTEPD_API::canvasDraw(this->_canvas_hash, this->_x, this->_y, UPDATE_MODE_DU4);
}

void HeapGauge::updateWithoutRender(){
    char* Text = (char*)malloc(sizeof(char) * 8);
    float Val_FreeHeapPercent = ((float)heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / (float)RAM_MAX) * 100.0;

    sprintf(Text, "%3.2f%%", Val_FreeHeapPercent);
    RTEPD_API::canvasSetText(this->_canvas_hash, 0, 0, 2, Text);
}