#pragma once
#include <Arduino.h>

struct TouchReference {
    float mean = 0;
    uint16_t min = 0;
    uint16_t max = 0;
};

enum class TouchEvent { None, Pressed, Released };

class TouchSensor {
public:
    explicit TouchSensor(uint8_t pin) : pin_(pin) {}
    uint16_t read();
    void calibrateIdle(const TouchReference& ref);
    bool calibrateTouch(const TouchReference& ref);
    TouchEvent update(uint16_t value, uint32_t now);
    bool calibrated() const { return idle_.mean > 0; }
    bool customThreshold() const { return custom_; }
    bool touched() const { return touched_; }
    float baseline() const { return idle_.mean; }
    float pressThreshold() const { return press_; }
    float releaseThreshold() const { return release_; }
    uint32_t duration(uint32_t now) const {
        return touched_ ? uint32_t(now - pressedAt_) : lastDuration_;
    }
private:
    uint8_t pin_;
    TouchReference idle_;
    float press_ = 0;
    float release_ = 0;
    bool custom_ = false;
    bool touched_ = false;
    bool candidate_ = false;
    uint32_t candidateAt_ = 0;
    uint32_t pressedAt_ = 0;
    uint32_t lastDuration_ = 0;
    void resetState();
};
