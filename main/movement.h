#pragma once
#include "DendoStepper.h"
#include "driver/gpio.h"
#include "pins.h"
#include "FreeRTOS/freertos.h"

extern uint32_t xTimeBeforeWhile;

#define WHILE_BLOCK(stat, timeout) {xTimeBeforeWhile = xTaskGetTickCount();while(stat){vTaskDelay(1);if(xTaskGetTickCount()>(xTimeBeforeWhile+(timeout/portTICK_PERIOD_MS))){break;}}}


typedef struct {
    int X=0;
    int Y=0;
    int Z=0;
} pos_t;

void initMov(DendoStepper *Xc, DendoStepper *Yc, DendoStepper *Zc);

int moveXY(int X, int Y);
void home(void*p);
int moveZ(int Z);
void getXYZ(int *X, int *Y, int *Z);