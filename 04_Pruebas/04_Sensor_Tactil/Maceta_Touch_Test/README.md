# Maceta Inteligente - Prueba táctil capacitiva V1

ESP32-WROOM-32, Arduino y PlatformIO. Usa el periférico touch nativo (T9).
No necesita bibliotecas externas, pantalla ni otros sensores.

## Conexión
Con la alimentación desconectada:
GPIO32 -- resistencia de 1 kΩ -- lámina de aluminio.
No conectes la lámina a 3.3 V ni a GND. Usa un cable corto y una película
aislante delgada sobre la lámina. Coloca la resistencia cerca del ESP32.
No uses una resistencia de 100 kΩ para esta conexión.

## Uso
Descomprime el ZIP, abre en Visual Studio Code la carpeta que contiene
platformio.ini, compila, carga y abre Monitor Serial a 115200 baudios.
Al arrancar, deja la lámina libre durante la calibración inicial.

En el ESP32 clásico, la lectura touch normalmente disminuye con el contacto.
La lectura es un contador interno, no una capacitancia en faradios.

## Calibración
- s: con la lámina libre, captura 40 muestras durante unos 1.2 s.
- t: mantén un dedo en la lámina y envía t sin soltar hasta ver la confirmación.
- r: imprime referencias, ruido pico a pico y umbrales.
- p: pausa/reanuda datos periódicos (los eventos siguen activos).
- ?: ayuda.

Si los rangos de reposo y contacto se separan, se calculan dos umbrales
distintos dentro del intervalo entre ambos. Si no, se conserva el umbral
anterior. Inicialmente se usa 80% de la referencia de reposo para detectar y
90% para liberar. Estos valores son provisionales.

Se exige que la condición de cambio permanezca al menos 80 ms de acuerdo
con las muestras tomadas aproximadamente cada 30 ms. La salida continua
se imprime cada 250 ms. Se registra duración desde el toque confirmado
hasta la liberación confirmada (no dura exactamente lo mismo que el contacto físico).
La captura de calibración es bloqueante, pero el ciclo normal no incluye
esperas largas.

## Pruebas
1. Comprueba estabilidad sin contacto.
2. Calibra con s y t; luego envía r y copia la salida.
3. Prueba toques breves y prolongados de 2, 5 y 10 segundos.
4. Comprueba que al soltar se detecte LIBRE.
5. Repite con la lámina en la posición final de la maceta y con la fuente
   de alimentación final; el entorno y el montaje pueden cambiar la respuesta.
6. Si hay falsos toques o no hay separación, revisa el tamaño de la lámina,
   cable, humedad y aislamiento. No cambies de pin todavía.

Las referencias se guardan solamente en RAM hasta reiniciar. No hay
clasificación final de caricias ni gestos, solo contacto, liberación y duración.
GPIO33 temperatura, GPIO34 humedad y pines de pantalla no se configuran.

Referencia: https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32/schematic-checklist.html
