#include "TouchSensor.h"

uint16_t TouchSensor::read() {
    return static_cast<uint16_t>(touchRead(pin_));
}

void TouchSensor::resetState() {
    touched_ = candidate_ = false;
    candidateAt_ = pressedAt_ = lastDuration_ = 0;
}

void TouchSensor::calibrateIdle(const TouchReference& ref) {
    idle_ = ref;
    custom_ = false;
    // ESP32 classic: reading decreases as capacitance increases.
    press_ = 0.80f * idle_.mean;
    release_ = 0.90f * idle_.mean;
    resetState();
}

bool TouchSensor::calibrateTouch(const TouchReference& ref) {
    if (!calibrated()) return false;
    // Require clear separation of the sampled ranges. Otherwise don't
    // overwrite a previously working threshold.
    if (ref.max >= idle_.min || idle_.mean - ref.mean < 3.0f) return false;
    const float gap = float(idle_.min) - float(ref.max);
    press_ = float(ref.max) + gap / 3.0f;
    release_ = float(ref.max) + gap * 2.0f / 3.0f;
    custom_ = true;
    resetState();
    return true;
}

TouchEvent TouchSensor::update(uint16_t value, uint32_t now) {
    if (!calibrated()) return TouchEvent::None;
    const bool next = touched_ ? value < release_ : value <= press_;
    if (next == touched_) {
        candidate_ = touched_;
        candidateAt_ = now;
        return TouchEvent::None;
    }
    if (next != candidate_) {
        candidate_ = next;
        candidateAt_ = now;
        return TouchEvent::None;
    }
    if (uint32_t(now - candidateAt_) < 80) return TouchEvent::None;
    touched_ = candidate_;
    if (touched_) {
        pressedAt_ = now;
        return TouchEvent::Pressed;
    }
    lastDuration_ = uint32_t(now - pressedAt_);
    return TouchEvent::Released;
}
