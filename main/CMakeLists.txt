idf_component_register(SRCS "movement.cpp" "endpoints.cpp" "main.cpp" "wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_http_server mcp23009 i2c-scanner DendoStepper nvs_flash mdns)