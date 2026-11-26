#include "TemperatureSensor.h"
#include <math.h>

TemperatureSensor::TemperatureSensor(uint8_t pin)
    : bus_(pin), sensor_(&bus_) {}

bool TemperatureSensor::begin() {
    sensor_.begin();
    pending_ = false;
    found_ = sensor_.getAddress(address_, 0);
    if (!found_ || address_[0] != 0x28) {
        found_ = false;
        return false;
    }
    sensor_.setResolution(address_, 12);
    sensor_.setWaitForConversion(false);
    return true;
}

bool TemperatureSensor::startConversion() {
    if (!found_ || pending_) return false;
    if (!sensor_.requestTemperaturesByAddress(address_)) {
        found_ = false;
        return false;
    }
    started_ = millis();
    pending_ = true;
    return true;
}

bool TemperatureSensor::ready() const {
    // 750 ms max at 12 bits; small timing margin.
    return pending_ && uint32_t(millis() - started_) >= 800;
}

bool TemperatureSensor::readCelsius(float& result) {
    if (!ready()) return false;
    pending_ = false;
    const float temp = sensor_.getTempC(address_);
    if (!isfinite(temp) || temp == DEVICE_DISCONNECTED_C ||
        temp < -55.0f || temp > 125.0f) {
        found_ = false;
        return false;
    }
    result = temp;
    return true;
}
