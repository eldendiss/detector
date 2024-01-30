#pragma once
#ifndef PINS_H
#define PINS_H

#define X_ENDSW_PIN GPIO_NUM_36
#define Y_ENDSW_PIN GPIO_NUM_35
#define Z_ENDSW_PIN GPIO_NUM_39

#define X_STEP 16
#define X_DIR 17
#define X_EN 15
#define Y_STEP 25
#define Y_DIR 26
#define Y_EN 15
#define Z_STEP 27
#define Z_DIR 32
#define Z_EN 15

#define X_STEPS_MM 160
#define Y_STEPS_MM 160
#define Z_STEPS_MM 160

#define MM_TO_DEG(x) (x/10)

#endif