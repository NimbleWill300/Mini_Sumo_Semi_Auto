#pragma once 

#include "util.hpp"
#include <stdint.h>

#define LEFT_MOTOR_PWM GPIO_NUM_15
#define RIGHT_MOTOR_PWM GPIO_NUM_18

#define MOTOR_IN_A1 GPIO_NUM_5
#define MOTOR_IN_A2 GPIO_NUM_17

#define MOTOR_IN_B1 GPIO_NUM_2
#define MOTOR_IN_B2 GPIO_NUM_4

typedef enum {
    RIGHT = 0,
    LEFT,
} side;

typedef struct motor {
    uint16_t right;
    uint16_t left;
} MotorSpeed;

void update_motors();

void motor_control(side side, int32_t speed);

