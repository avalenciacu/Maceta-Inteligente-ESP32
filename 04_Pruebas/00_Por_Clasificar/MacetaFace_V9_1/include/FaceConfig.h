#pragma once
#include <Arduino.h>
namespace FaceConfig {
constexpr int WIDTH=240, HEIGHT=240, STRIP=16, STRIPS=15;
constexpr uint32_t FRAME_MS=25, BROW_MIN=6000, BROW_MAX=15000;
constexpr uint32_t MOOD_MIN=10000, MOOD_MAX=20000;
constexpr float EYE_RADIUS=26, PUPIL_RADIUS=19.5f;
constexpr float FACE_X=36*.85f, FACE_Y=41*.85f;
constexpr float SMILE_WIDTH=1.30f;
}
