#pragma once
#include <Arduino.h>

struct SoilReading {
    uint16_t raw;
    uint16_t average;
    uint16_t minimum;
    uint16_t maximum;
};

class SoilSensor {
public:
    explicit SoilSensor(uint8_t pin) : pin_(pin) {}
    void begin();
    SoilReading read();
private:
    uint8_t pin_;
};
