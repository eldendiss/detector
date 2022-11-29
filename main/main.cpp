#include <stdio.h>
#include "endpoints.h"
#include "DendoStepper.h"
#include "driver/i2c.h"
#include "i2c-scanner.h"
#include "mcp23009.h"
#include "pins.h"
#include "movement.h"

extern "C"
{
#include "wifi.h"
}

DendoStepper X;
DendoStepper Y;
DendoStepper Z;
mcp23009 mcp;

extern "C" void app_main(void)
{

    initWifi();
    startServer();
    initMov(&X,&Y,&Z);
    home(NULL);
    ESP_LOGW("main","homed");

    while (1)
    {
        vTaskDelay(10);
    }
}