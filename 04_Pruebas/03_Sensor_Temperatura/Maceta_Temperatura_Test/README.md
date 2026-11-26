# Maceta Inteligente - Prueba DS18B20 V1

Proyecto independiente de PlatformIO para ESP32-WROOM-32 (esp32dev).

## Conexion (con la alimentacion desconectada)

DS18B20 TO-92, cara plana hacia ti, patas hacia abajo:
1. GND -> GND comun.
2. DATA -> GPIO23.
3. VDD -> 3.3 V.
Resistencia de 4.7 kOhm entre DATA y 3.3 V (pull-up, no en serie).

El programa solo usa GPIO23 y el puerto serial normal. No inicializa la
pantalla GC9A01, el FC-28 ni el sensor tactil. Pueden permanecer conectados.

## Uso

Descomprime el ZIP, abre en VS Code la carpeta que contiene platformio.ini,
compila, carga y abre el monitor serial a 115200 baudios. PlatformIO
instalara OneWire y DallasTemperature automaticamente la primera vez
(requiere internet si no estan en la cache).

El programa muestra una lectura cada 2 segundos y los extremos registrados
desde el arranque. La conversion a 12 bits se realiza sin esperar de forma
bloqueante; se dejan 800 ms antes de leerla (el maximo especificado es
750 ms). La salida muestra 4 decimales para apreciar los pasos de 0.0625 C,
no porque la exactitud sea de cuatro decimales.

## Prueba funcional

1. Deja estabilizar el sensor a temperatura ambiente, lejos del ESP32,
   del display y de fuentes de calor.
2. Acerca la mano o un recipiente cerrado con agua tibia, sin tocar
   conexiones electricas. Espera a observar un aumento.
3. Retira el calor y deja recuperar la temperatura ambiente.
4. Acerca un recipiente frio cerrado, evitando condensacion sobre la
   electronica. Espera a observar una disminucion.
5. Registra temperaturas estabilizadas para cada condicion y comparte
   una captura del monitor.

No uses llamas, agua hirviendo ni sumerjas un TO-92 desnudo.
El DS18B20 viene calibrado: esta es una prueba de respuesta, no una
calibracion metrologica. El codigo no define umbrales de frio o calor.
Lecturas invalidas se descartan y se reintenta la deteccion cada 2 s.
85 C repetidos a temperatura ambiente pueden indicar reinicios del sensor.

Fuentes:
- https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
- https://github.com/milesburton/Arduino-Temperature-Control-Library
- https://github.com/PaulStoffregen/OneWire
