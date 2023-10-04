#pragma once 

#include <PS4Controller.h>
#include "state.hpp"
#include "motors.hpp"

typedef struct{
  int r = 255;
  int g = 255;
  int b = 255;
}LedColors;

int get_motor_power(side lado);

void set_motor_power(side lado, int valor);

void change_led_color(int r, int g, int b);

int calculate_deadZone(int input);

int update_controller(ControllerState state);