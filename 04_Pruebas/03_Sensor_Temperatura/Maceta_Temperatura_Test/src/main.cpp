#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DATA_PIN 33

OneWire oneWire(DATA_PIN);
DallasTemperature sensors(&oneWire);

void setup() {

    Serial.begin(115200);
    sensors.begin();
    sensors.setResolution(12);

    Serial.println("PRUEBA DS18B20 - ESP32");
}

void loop() {

    sensors.begin();

    int cantidad = sensors.getDeviceCount();

    Serial.print("Sensores detectados: ");
    Serial.println(cantidad);

    if (cantidad == 0) {

        Serial.println("ERROR: Sensor no detectado");
        delay(2000);
        return;
    }

    sensors.requestTemperatures();

    float temperatura = sensors.getTempCByIndex(0);

    if (temperatura == DEVICE_DISCONNECTED_C) {

        Serial.println("ERROR: Sensor desconectado");

    } else {

        Serial.print("Temperatura: ");
        Serial.print(temperatura, 4);
        Serial.println(" C");
    }

    delay(2000);
}