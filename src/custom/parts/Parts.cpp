#include "./Parts.h"

Parts::Parts(uint16_t x, uint16_t y, CustomSetups* setup, PriorityNum priority){
    this->setPos(x, y);
    this->_setup = setup;

    if(!this->_flag_debug) return;
}

void Parts::init(){
    this->_flag_disable = false;
    this->_flag_stop_render = false;
}

void Parts::update(){
}

void Parts::updateWithoutRender(){
}

void Parts::setPos(uint16_t x, uint16_t y){
    this->_x = x;
    this->_y = y;
}

void Parts::setDisable(bool flag_disable){
    this->_flag_disable = flag_disable;
}

bool Parts::isReadyToUpdate(){
    if((millis() - this->_millis_last_update) > this->_millis_update_span) return true;
    else return false;
}

void Parts::setLastUpdate(){
    this->_millis_last_update = millis();
}

void Parts::debugPrint(String mes){
    if(!_flag_debug) return;

    Serial.println(mes);
}


// static methods

void Parts::updates(std::vector<Parts*> parts){
    for(uint32_t i = 0; i < parts.size(); i++){
        // Serial.println("1A");
        if(parts[i] == nullptr) continue;
        // Serial.println("1B");

        parts[i]->update();
        // Serial.println("1C");
    }
}

void Parts::setDisables(std::vector<Parts*> parts){
    for(uint32_t i = 0; i < parts.size(); i++){
        parts[i]->setDisable(false);
    }
}

void Parts::inits(std::vector<Parts*> parts){
    for(uint32_t i = 0; i < parts.size(); i++){
        parts[i]->init();
    }
}

bool Parts::createHash(char* hash, uint8_t buffSize){
    const static char CharList[63] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint8_t LoopCnt_i;

    if(hash == nullptr) return false;

    for(LoopCnt_i = 0; LoopCnt_i < (buffSize - 1); LoopCnt_i++){
        *(hash + LoopCnt_i) = CharList[random(0, 61)];
    }
    *(hash + LoopCnt_i) = '\0';

    return true;
}