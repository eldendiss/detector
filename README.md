# Detector

Barebone code for 3D positioning

Requires [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32/get-started/index.html) version 5.0.0 or higher

# Misc settings

WiFi credentials (SSID, password) can be set in wifi.h

Stepper motor steps per milimeter and pinout can be set in pins.h

Server APIs handlers are inside endpoints.cpp

Stepper motors are controlled by [DendoStepper](https://github.com/eldendiss/DendoStepper) library