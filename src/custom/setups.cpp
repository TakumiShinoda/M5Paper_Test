#include "./setups.h"

void CustomSetups::Wifi(){
    uint16_t Cnt_WifiFailed = 0;

    WiFi.disconnect();
    delay(1000);
    WiFi.begin(SSID, PASS);
    delay(500);

    while(
        (WiFi.status() != WL_CONNECTED) &&
        (Cnt_WifiFailed < 15)
    ){
		Serial.print(".");
        Cnt_WifiFailed++;
        delay(500);
	}
    Serial.println("");

    if(WiFi.status() != WL_CONNECTED){
        Serial.println("WiFi Connection Failed");
        sleep(65535);
    }

    Serial.print("\nIP address: ");
	Serial.println(WiFi.localIP());
}

void CustomSetups::MMC_SD(){
    pinMode(4, OUTPUT);
    SPI.begin(14, 13, 12);

	if(!SD.begin(4)){
		Serial.println("SD Card Mount Failed");
	}else{
        Serial.println("SD: Success");
    }
}

void CustomSetups::TP(){
    M5.TP.SetRotation(GT911::ROTATE_90);
    M5.TP.begin(21, 22, 36);

    this->_IsAvailable_TP = true;
}

void CustomSetups::EPD(){
    pinMode(M5EPD_EPD_PWR_EN_PIN, OUTPUT);
    M5.enableEPDPower();
    delay(1000);

    M5.EPD.begin(M5EPD_SCK_PIN, M5EPD_MOSI_PIN, M5EPD_MISO_PIN, M5EPD_CS_PIN, M5EPD_BUSY_PIN);
    M5.EPD.Clear(true);
    delay(30);

    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    this->_IsAvailable_EPD = true;
}

void CustomSetups::RTC(){
    rtc_date_t Time_Date;
    rtc_time_t Time_Now;

    M5.RTC.begin();
    this->_IsAvailable_RTC = true;

    // M5.RTC.getDate(&Time_Date);
    // M5.RTC.getTime(&Time_Now);

    // randomSeed(CustomSetups::getEpoch(&Time_Date, &Time_Now));
}

bool CustomSetups::isAvailableEPD(){
    return this->_IsAvailable_EPD;
}

bool CustomSetups::isAvailableTP(){
    return this->_IsAvailable_TP;
}

bool CustomSetups::isAvailableRTC(){
    return this->_IsAvailable_RTC;
}

void CustomSetups::update(){
    this->_flag_tp_available = M5.TP.avaliable();
    
    if(this->_flag_tp_available){
        M5.TP.update();
        this->_flag_tp_fingerup = M5.TP.isFingerUp();
    }else{
        this->_flag_tp_fingerup = false;
    }
}

bool CustomSetups::getAvailable_TP(){
    return this->_flag_tp_available;
}

bool CustomSetups::getIsFingerUp_TP(){
    return this->_flag_tp_fingerup;
}

uint64_t CustomSetups::getEpoch(rtc_date_t* date, rtc_time_t* time){
    tm Tm_Now;

    Tm_Now.tm_year = date->year - 1900;
    Tm_Now.tm_mon = date->mon;
    Tm_Now.tm_mday = date->day;
    Tm_Now.tm_hour = time->hour;
    Tm_Now.tm_min = time->min;
    Tm_Now.tm_sec = time->sec;

    return (uint64_t)mktime(&Tm_Now);
}