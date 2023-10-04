#pragma once

#include <PS4Controller.h>
#include "controller.hpp"

enum State{
  Controller,
  Square,
  Circle
};

enum ControllerState{
  NormalMode,
  ProMode
};

void update_state();

void run_state();

int get_state();

void set_state(State novo_estado);