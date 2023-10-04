#pragma once 

#include <stdint.h>

#define MAX_MOTOR 255
#define MOTOR_DEADZONE 20


#define EN_B GPIO_NUM_15
#define BI1 GPIO_NUM_2
#define BI2 GPIO_NUM_4
#define STANDBY GPIO_NUM_16
#define AI2 GPIO_NUM_17
#define AI1 GPIO_NUM_5
#define EN_A GPIO_NUM_18

typedef enum {
    RIGHT = 0,
    LEFT,
} side;

typedef struct motor {
    uint16_t right;
    uint16_t left;
} MotorSpeed;

typedef struct{
  int powerRight;
  int powerLeft;
}Motors;

void motor_setup();

void motor_control(side side, int32_t speed);

