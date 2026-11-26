# Maceta Inteligente — Prueba de humedad V1

Proyecto independiente para ESP32 clásico, PlatformIO y Arduino. No requiere
bibliotecas externas. No modifica MacetaFace ni inicializa la pantalla.
Solo se configura GPIO34 para el sensor; se usa el puerto serial habitual.

## Conexión FC-28 / LM393
- A0 -> GPIO34 (ADC1)
- VCC -> 3.3 V (no 5 V)
- GND -> GND común
- D0 -> sin conectar

La pantalla puede quedar conectada a su alimentación, pero no se usa en el
programa. No se configura ningún pin de control de pantalla.

## Abrir
Descomprime el ZIP, abre la carpeta que contiene platformio.ini en VS Code
con PlatformIO instalado, selecciona Build y Upload. Abre el monitor serial
a 115200 baudios. En la primera compilación PlatformIO puede necesitar bajar
el framework y compilador si todavía no están instalados.

## Datos
Cada 500 ms se imprimen RAW (última lectura), PROM (media de 16 muestras),
MIN, MAX y RUIDO (MAX-MIN, rango de muestras, no desviación estándar).
Las lecturas nominales ADC son de 0 a 4095. Los límites no necesariamente
representan 0 V ni 3.3 V exactos. La salida es un índice bruto, no un porcentaje
de contenido de agua.

## Calibración
Espera a que el promedio se estabilice en cada condición y envía una letra
desde el monitor serial:
- a: sonda al aire (limpia y seca).
- s: sonda en una muestra de tierra seca.
- h: sonda en tierra con humedad adecuada para la planta.
- m: sonda en una muestra separada de tierra saturada.
- p: pausa/reanuda la impresión periódica (los comandos siguen activos).
- r: muestra las referencias guardadas en RAM.
- ?: ayuda.

Usa el mismo tipo de tierra y profundidad en las tres pruebas de suelo.
No empapes el módulo electrónico ni inundes la planta para calibrar.
Si una lectura no es estable, vuelve a capturarla. Una nueva captura de la
misma letra reemplaza la anterior. Copia el resumen antes de reiniciar,
pues las referencias no se guardan en memoria permanente.
El porcentaje y los umbrales de alerta se definirán después de revisar los
datos reales. El potenciómetro del LM393 normalmente ajusta D0, no A0.

El sensor resistivo puede corroerse si permanece alimentado continuamente;
por ahora desconéctalo al terminar las pruebas.
