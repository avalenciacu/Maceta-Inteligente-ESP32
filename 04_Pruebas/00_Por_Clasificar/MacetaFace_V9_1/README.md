# MacetaFace Sphere Star V9.1 — prueba ordenada de 52 actuaciones

Proyecto PlatformIO para ESP32-WROOM-32 y GC9A01 240 x 240. Basado en la V7 que funcionó en la pantalla del usuario. Conserva platformio.ini, pines y TFT_eSPI. Sin sensores, sin sonido, sin suspensión real en esta prueba.

## Primera prueba
1. Descomprime el ZIP y abre la carpeta MacetaFace_V9_1 en VS Code (la carpeta que contiene platformio.ini).
2. Conserva el proyecto V7 como respaldo. Compila con PlatformIO Build y sube con Upload.
3. Abre el monitor serie a 115200 baudios. Se imprime el número y nombre de cada actuación. Cada una dura 5000 ms desde su selección. Se vuelve a Normal después de la 52.
4. Una vuelta nominal dura 260 s (4 min 20 s); puede tardar algo más por el tiempo de renderizado.
5. El archivo CATALOGO.txt contiene el orden.

## Configuración
VCC y BLK a 3.3 V, GND a GND; SCL 14, SDA 13, DC 27, CS 26, RST 25. SPI HSPI 20 MHz. No modificar conexiones si V7 ya funciona.

## Archivos
- include/: interfaces y configuración del motor.
- src/MacetaFace.cpp, FaceAnimation.cpp, FaceBehavior.cpp, FaceExpressions.cpp, FaceRenderer.cpp, FaceActivities.cpp y FaceObjects.cpp: implementación gráfica. Conservar estos archivos al modificar el comportamiento del proyecto.
- src/main.cpp: demostración ordenada. En el futuro se reemplaza únicamente este archivo y se agregan módulos de sensores/control según necesidad; no borrar los otros .cpp.

## API
```cpp
face.setMood(Mood::HAPPY); // reproducción con tiempo natural
face.setMood(Mood::DRINKING, 5000); // comprimir/expandir actuación a 5 s
face.update(); // llamar continuamente desde loop
face.setRandomBrows(true);
face.setAutoBlink(true);
face.setAutoLook(true);
face.setAutoGestures(true);
face.look(-1,0);
face.blink();
```
La demo usa el segundo argumento de setMood para ajustar la duración de actuaciones largas, incluido bostezo, despertar y objetos. Las cejas cambian de forma de manera independiente cada 6–15 s; el temporizador no se reinicia en cada cambio, salvo selección del estilo escéptico de V7. Los parpadeos y miradas automáticas mantienen sus temporizadores. La alerta curiosa recorre lados y arriba/abajo; despertar tiene apertura progresiva, dos parpadeos y sonrisa. Las notas y signos de interrogación se dibujan con primitivas gruesas, sin fuentes adicionales.

## Límites de verificación
Compilación C++11 y ejecución de una ronda completa con sustitutos de Arduino/TFT_eSPI en PC, comprobando límites de escritura del sprite. No se compiló con PlatformIO real ni se cargó en ESP32. Las figuras SVG de los simuladores se aproximan con primitivas RGB565: no hay equivalencia exacta de antialiasing, transparencia, rotaciones o geometría. Las actuaciones nuevas requieren revisión visual física. El búfer de píxeles permanece en 7680 bytes; no se ha medido el consumo real total ni FPS. No se almacenan imágenes completas. Los sensores y la lógica de necesidades no están incluidos aún.
