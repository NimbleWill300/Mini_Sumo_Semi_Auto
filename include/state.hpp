#pragma once

#include <PS4Controller.h>
#include "controller.hpp"

enum State{
  Controller,
  SquareLeft,
  SquareRight,
  RightDodge,
  LeftDodge,
  L1,
  R1
};

void update_state();

void run_state();

int get_state();

void set_state(State novo_estado);