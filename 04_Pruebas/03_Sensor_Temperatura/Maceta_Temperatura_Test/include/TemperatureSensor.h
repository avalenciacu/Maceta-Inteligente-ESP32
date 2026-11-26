#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor {
public:
    explicit TemperatureSensor(uint8_t pin);
    bool begin();
    bool startConversion();
    bool ready() const;
    bool readCelsius(float& result);

private:
    OneWire bus_;
    DallasTemperature sensor_;
    DeviceAddress address_ = {};
    bool found_ = false;
    bool pending_ = false;
    uint32_t started_ = 0;
};
