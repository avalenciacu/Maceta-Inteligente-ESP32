#include <Arduino.h>
#include "TouchSensor.h"

constexpr uint8_t TOUCH_PIN = 32;
constexpr uint32_t SAMPLE_MS = 30;
constexpr uint32_t PRINT_MS = 250;
TouchSensor touch(TOUCH_PIN);
TouchReference idleRef;
TouchReference touchRef;
bool hasTouchRef = false;
bool live = true;
uint32_t lastSample = 0;
uint32_t lastPrint = 0;

TouchReference capture() {
    TouchReference ref;
    ref.min = 65535;
    uint32_t sum = 0;
    constexpr uint8_t N = 40;
    for (uint8_t i = 0; i < N; i++) {
        const uint16_t v = touch.read();
        sum += v;
        if (v < ref.min) ref.min = v;
        if (v > ref.max) ref.max = v;
        delay(30);
    }
    ref.mean = float(sum) / N;
    return ref;
}

void printRef(const char* label, const TouchReference& ref) {
    Serial.printf("%s | PROM=%.2f | MIN=%u | MAX=%u | RANGO=%u\n",
                  label, ref.mean, (unsigned)ref.min, (unsigned)ref.max,
                  (unsigned)(ref.max - ref.min));
}

void report() {
    Serial.println("\n=== REFERENCIAS TOUCH GPIO32 ===");
    printRef("SIN TOCAR", idleRef);
    if (hasTouchRef) printRef("TOCADO", touchRef);
    else Serial.println("TOCADO: pendiente (comando t)");
    if (touch.calibrated()) {
        Serial.printf("UMBRAL TOQUE=%.2f | UMBRAL SOLTAR=%.2f | %s\n",
                      touch.pressThreshold(), touch.releaseThreshold(),
                      touch.customThreshold() ? "CALIBRADO" : "PROVISIONAL");
    } else {
        Serial.println("ERROR: referencia de reposo nula. Revisar electrodo.");
    }
    Serial.println("==============================");
}

void calibrateIdle() {
    Serial.println("\nNO TOQUES LA LAMINA durante la captura...");
    idleRef = capture();
    hasTouchRef = false;
    touch.calibrateIdle(idleRef);
    report();
}

void calibrateTouched() {
    Serial.println("\nMANTEN EL DEDO en la lamina durante la captura...");
    const TouchReference measured = capture();
    printRef("MEDICION TOCADA", measured);
    if (touch.calibrateTouch(measured)) {
        touchRef = measured;
        hasTouchRef = true;
        Serial.println("Calibracion aceptada. Ya puedes soltar.");
    } else {
        Serial.println("Calibracion rechazada: poca separacion o rangos superpuestos.");
        Serial.println("Repetir s y t, manteniendo el dedo durante t.");
    }
    report();
}

void help() {
    Serial.println("s = capturar sin tocar");
    Serial.println("t = capturar con el dedo (mantener hasta fin)");
    Serial.println("r = mostrar referencias y umbrales");
    Serial.println("p = pausar/reanudar impresion continua");
    Serial.println("? = ayuda");
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nMACETA INTELIGENTE - TEST TOUCH V1");
    Serial.println("GPIO32 / T9 | Sin pantalla ni otros sensores");
    help();
    Serial.println("Deja la lamina libre. Captura inicial en 2 segundos.");
    delay(2000);
    calibrateIdle();
}

void loop() {
    while (Serial.available()) {
        switch (Serial.read()) {
            case 's': case 'S': calibrateIdle(); break;
            case 't': case 'T': calibrateTouched(); break;
            case 'r': case 'R': report(); break;
            case 'p': case 'P':
                live = !live;
                Serial.println(live ? "Lecturas reanudadas" : "Lecturas pausadas");
                break;
            case '?': help(); break;
            case '\r': case '\n': case ' ': break;
            default: Serial.println("Comando desconocido. Usa ?."); break;
        }
    }
    if (uint32_t(millis() - lastSample) >= SAMPLE_MS) {
        lastSample = millis();
        uint16_t raw = touch.read();
        uint32_t now = millis();
        TouchEvent event = touch.update(raw, now);
        if (event == TouchEvent::Pressed) Serial.println(">>> TOQUE DETECTADO");
        if (event == TouchEvent::Released)
            Serial.printf(">>> SOLTADO | DURACION=%lu ms\n", (unsigned long)touch.duration(now));
        if (live && uint32_t(now - lastPrint) >= PRINT_MS) {
            lastPrint = now;
            if (touch.calibrated()) {
                Serial.printf("RAW=%u | BASE=%.2f | TOQUE<=%.2f | SOLTAR>=%.2f | %s",
                              (unsigned)raw, touch.baseline(), touch.pressThreshold(),
                              touch.releaseThreshold(), touch.touched() ? "TOCADO" : "LIBRE");
                if (touch.touched())
                    Serial.printf(" | TIEMPO=%lu ms", (unsigned long)touch.duration(now));
                Serial.println();
            } else {
                Serial.printf("RAW=%u | SIN CALIBRAR\n", (unsigned)raw);
            }
        }
    }
    delay(1);
}
