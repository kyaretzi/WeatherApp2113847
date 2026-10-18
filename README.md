# WeatherApp 2113847

Estación meteorológica IoT con ESP32 y sensor BMP280. Mide temperatura, presión y altitud, y las envía a Firebase Realtime Database para mostrarlas en una aplicación web.

## Hardware
- ESP32
- Sensor BMP280 (I2C, dirección 0x77)
- LED indicador en el pin 33

## Bibliotecas
- Adafruit BMP280
- Firebase ESP32 (incluida en `lib/FIREBASE-ESP32`)

## Configuración
1. Copie `src/secrets.example.h` como `src/secrets.h`.
2. Escriba en `secrets.h` su WiFi, el host de Firebase y su clave.
3. Compile y cargue con PlatformIO.

## Estructura de Firebase
Nodo `/Weather` con las claves `Temperatura` (°C), `Presion` (mmHg) y `Altitud` (m).

## Autor
AlexAmaya19