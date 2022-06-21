# Esp32_MQTT_JSON_WiFi

A project 2 ESP32-őn működik. Az egyik küldi a JSON adatot adafruit IO MQTT-n keresztül, a másik fogadja.
JSON-ből kiolvassa, hogy melyik GPIO pin-t kell, hogy felkapcsolja illetve egy rövid idő után kikapcsolja magát.
Az ESP32 az internetre/modemre a WiFimanager segítségével történik, nincsen hardcodolva az SSID, PASS.
Valamint alkalmazva van a SPIFFS is, aminek a célja, hogy egy dummy data-t elmentsen és megjelenítsen ami a JSON fileban található.
A project az Arduino frameworkben van írva. A későbbiekben meg az ESP-IDF frameworkjével fogok foglalkozni/tanulni.

