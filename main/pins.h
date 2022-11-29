#pragma once
#ifndef PINS_H
#define PINS_H

#define X_ENDSW_PIN GPIO_NUM_35
#define Y_ENDSW_PIN GPIO_NUM_34
#define Z_ENDSW_PIN GPIO_NUM_23

#define X_STEP 14
#define X_DIR 27
#define X_EN 26
#define Y_STEP 13
#define Y_DIR 12
#define Y_EN 26
#define Z_STEP 25
#define Z_DIR 33
#define Z_EN 26

#define X_STEPS_MM 80
#define Y_STEPS_MM 80
#define Z_STEPS_MM 10

#define MM_TO_DEG(x) (x/10)

#endif