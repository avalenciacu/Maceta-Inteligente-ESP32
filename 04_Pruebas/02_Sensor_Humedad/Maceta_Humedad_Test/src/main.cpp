
#include <Arduino.h>
#include "SoilSensor.h"

// ==========================================
// CONFIGURACION DEL SENSOR
// ==========================================

constexpr uint8_t SOIL_PIN = 34;

constexpr int ADC_SECO = 3870;
constexpr int ADC_SATURADO = 1419;

constexpr unsigned long INTERVALO_LECTURA = 500;
constexpr unsigned long TIEMPO_CONFIRMACION = 5000;

constexpr float ALPHA = 0.25f;
constexpr float HISTERESIS = 2.0f;

SoilSensor soil(SOIL_PIN);

// ==========================================
// ESTADOS DE HUMEDAD
// ==========================================

enum EstadoHumedad {
    SED_INTENSA,
    NECESITA_AGUA,
    HUMEDAD_BAJA,
    HUMEDAD_ADECUADA,
    HUMEDAD_ELEVADA,
    EXCESO_AGUA
};

constexpr float UMBRALES[] = {
    10.0f,
    25.0f,
    32.0f,
    60.0f,
    80.0f
};

EstadoHumedad estadoActual = HUMEDAD_ADECUADA;
EstadoHumedad estadoPendiente = HUMEDAD_ADECUADA;

float adcFiltrado = 0.0f;
float humedad = 0.0f;

bool primeraLectura = true;

unsigned long ultimaLectura = 0;
unsigned long inicioCambio = 0;

// ==========================================
// NOMBRE DE LOS ESTADOS
// ==========================================

const char* nombreEstado(EstadoHumedad estado) {

    switch (estado) {

        case SED_INTENSA:
            return "SED INTENSA";

        case NECESITA_AGUA:
            return "NECESITA AGUA";

        case HUMEDAD_BAJA:
            return "HUMEDAD BAJA";

        case HUMEDAD_ADECUADA:
            return "HUMEDAD ADECUADA";

        case HUMEDAD_ELEVADA:
            return "HUMEDAD ELEVADA";

        case EXCESO_AGUA:
            return "EXCESO DE AGUA";

        default:
            return "DESCONOCIDO";
    }
}

// ==========================================
// CONVERSION ADC A HUMEDAD RELATIVA
// ==========================================

float calcularHumedad(float adc) {

    float porcentaje =
        (ADC_SECO - adc) * 100.0f /
        (ADC_SECO - ADC_SATURADO);

    return constrain(porcentaje, 0.0f, 100.0f);
}

// ==========================================
// CLASIFICACION INICIAL
// ==========================================

EstadoHumedad clasificarHumedad(float h) {

    if (h < 10.0f)
        return SED_INTENSA;

    if (h < 25.0f)
        return NECESITA_AGUA;

    if (h < 32.0f)
        return HUMEDAD_BAJA;

    if (h < 60.0f)
        return HUMEDAD_ADECUADA;

    if (h < 80.0f)
        return HUMEDAD_ELEVADA;

    return EXCESO_AGUA;
}

// ==========================================
// CLASIFICACION CON HISTERESIS
// ==========================================

EstadoHumedad clasificarConHisteresis(float h) {

    int estado = static_cast<int>(estadoActual);

    // Incremento de humedad
    while (estado < EXCESO_AGUA &&
           h >= UMBRALES[estado] + HISTERESIS) {

        estado++;
    }

    // Disminucion de humedad
    while (estado > SED_INTENSA &&
           h < UMBRALES[estado - 1] - HISTERESIS) {

        estado--;
    }

    return static_cast<EstadoHumedad>(estado);
}

// ==========================================
// CONFIRMACION DE CAMBIO DE ESTADO
// ==========================================

void actualizarEstado(float h) {

    EstadoHumedad nuevoEstado =
        clasificarConHisteresis(h);

    unsigned long ahora = millis();

    if (nuevoEstado == estadoActual) {

        estadoPendiente = estadoActual;
        inicioCambio = ahora;
        return;
    }

    if (nuevoEstado != estadoPendiente) {

        estadoPendiente = nuevoEstado;
        inicioCambio = ahora;
        return;
    }

    if (ahora - inicioCambio >= TIEMPO_CONFIRMACION) {

        estadoActual = estadoPendiente;

        Serial.println();
        Serial.println("============================");

        Serial.print("NUEVO ESTADO: ");
        Serial.println(nombreEstado(estadoActual));

        Serial.println("============================");
    }
}

// ==========================================
// LECTURA Y PROCESAMIENTO
// ==========================================

void procesarSensor() {

    SoilReading lectura = soil.read();

    if (primeraLectura) {

        adcFiltrado = lectura.average;
        humedad = calcularHumedad(adcFiltrado);

        estadoActual = clasificarHumedad(humedad);
        estadoPendiente = estadoActual;

        inicioCambio = millis();
        primeraLectura = false;

    } else {

        // Filtro exponencial EMA
        adcFiltrado =
            ALPHA * lectura.average +
            (1.0f - ALPHA) * adcFiltrado;

        humedad = calcularHumedad(adcFiltrado);

        actualizarEstado(humedad);
    }

    // Datos del monitor serial
    Serial.printf(
        "ADC=%u | FILTRO=%.1f | HUMEDAD=%.2f%%"
        " | RUIDO=%u | ESTADO=%s",
        (unsigned)lectura.average,
        adcFiltrado,
        humedad,
        (unsigned)(lectura.maximum - lectura.minimum),
        nombreEstado(estadoActual)
    );

    if (estadoPendiente != estadoActual) {

        Serial.print(" | CONFIRMANDO: ");
        Serial.print(nombreEstado(estadoPendiente));
    }

    Serial.println();
}

// ==========================================
// SETUP
// ==========================================

void setup() {

    Serial.begin(115200);

    soil.begin();

    Serial.println();
    Serial.println("==================================");
    Serial.println(" MACETA INTELIGENTE - HUMEDAD V2");
    Serial.println("==================================");

    Serial.println("Sensor: FC-28");
    Serial.println("Pin: GPIO34");
    Serial.println("ADC seco: 3870");
    Serial.println("ADC saturado: 1419");

    Serial.println("Filtro EMA: ACTIVADO");
    Serial.println("Histeresis: ACTIVADA");
    Serial.println("Confirmacion: 5 segundos");
    Serial.println("Pantalla: NO UTILIZADA");

    Serial.println("==================================");

    procesarSensor();
    ultimaLectura = millis();
}

// ==========================================
// LOOP
// ==========================================

void loop() {

    unsigned long ahora = millis();

    if (ahora - ultimaLectura >= INTERVALO_LECTURA) {

        ultimaLectura = ahora;

        procesarSensor();
    }

    delay(1);
}
