#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp_system.h>
#include "MacetaFace.h"

TFT_eSPI tft;
MacetaFace face(tft);

constexpr uint32_t STEP_MS = 5000;

static_assert((int)Mood::COUNT == 52, 
              "Catalogo incompleto");

static bool ready = false;
static unsigned moodIndex = 0;
static uint32_t started = 0;

void show(unsigned i) {
    moodIndex = i % 52;

    face.setMood(
        static_cast<Mood>(moodIndex),
        STEP_MS
    );

    started = millis();

    Serial.printf(
        "%02u/52 - %s\n",
        moodIndex + 1,
        moodName(face.mood())
    );
}

void setup() {
    Serial.begin(115200);
    randomSeed(esp_random());

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    ready = face.begin();

    if (!ready) {
        Serial.println("ERROR sprite RGB565");
        tft.fillScreen(TFT_RED);
        return;
    }

    face.setRandomBrows(true);
    face.setAutoBlink(true);
    face.setAutoLook(true);
    face.setAutoGestures(true);

    show(0);
}

void loop() {
    if (!ready) {
        delay(100);
        return;
    }

    if (millis() - started >= STEP_MS) {
        show(moodIndex + 1);
    }

    face.update();
    yield();
}