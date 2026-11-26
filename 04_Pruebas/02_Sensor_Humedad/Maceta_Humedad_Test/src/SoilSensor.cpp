#include "SoilSensor.h"

void SoilSensor::begin() {
    pinMode(pin_, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(pin_, ADC_11db);
}

SoilReading SoilSensor::read() {
    constexpr uint8_t count = 16;
    SoilReading out = {0, 0, 4095, 0};
    uint32_t total = 0;
    for (uint8_t i = 0; i < count; ++i) {
        out.raw = analogRead(pin_);
        total += out.raw;
        if (out.raw < out.minimum) out.minimum = out.raw;
        if (out.raw > out.maximum) out.maximum = out.raw;
        if (i + 1 < count) delay(2);
    }
    out.average = (total + count / 2) / count;
    return out;
}
