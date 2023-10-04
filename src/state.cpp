#include "state.hpp"

State ultimo_estado = estado;
State estado = Controller;
ControllerState controllerstate = NormalMode;
int should_change = 0;
int move_step = 0;
int change_move_time;

int const num_square_steps = 4;
int square_attack[num_square_steps][3]{
      {-255, 255, 60},    //vira esquerda
      {255, 255, 150},     //reto
      {255, -255, 120},     //vira direita
      {255, 255, 150}      //ret0
};

int const num_circle_steps = 4;
int circle_attack[num_circle_steps][3]{
      {255, -255, 60},    //vira direita
      {255, 255, 150},     //reto
      {-255, 255, 120},     //vira esquerda
      {255, 255, 150}      //ret0
};

void update_state(){
  if(update_controller(controllerstate)){         //verifica se o controle foi movido, pois se sim
    estado = Controller;                          //cancela o movimento e volta ao modo controlado
    return;
  }
  if(PS4.Square() && estado != Square){           //ataque lateral pela esquerda
    estado = Square;
    should_change = 0;
    change_move_time = millis() + square_attack[0][2];
    move_step = 0;
  }
  if(PS4.Circle() && estado != Circle){           //ataque lateral pela direita
    estado = Circle;
    should_change = 0;
    change_move_time = millis() + circle_attack[0][2];
    move_step = 0;
  }
  if(PS4.R3() && controllerstate != ProMode){
    controllerstate = ProMode;
  }
  if(PS4.L3() && controllerstate != NormalMode){
    controllerstate = NormalMode;
  }
}

void run_state(){
  if(estado == Controller){
    if(controllerstate == NormalMode){
      change_led_color(0, 255, 0);      //Verde
    }else{
      change_led_color(255, 0, 0);    //Vermelho
    }
    motor_control(LEFT, get_motor_power(RIGHT));
    motor_control(RIGHT, get_motor_power(LEFT));
    return;
  }

  int num_steps;
  int (*attack)[3];
  switch(estado){
    case Square:
      change_led_color(220, 20, 60);
      num_steps = num_square_steps;
      attack = square_attack;
      break;
    case Circle:
      change_led_color(60, 220, 20);
      num_steps = num_circle_steps;
      attack = circle_attack;
      break;
  }
  
  if(should_change){
    should_change = 0;
    // Serial.println(move_step);
    if(move_step < num_steps){
      move_step++;
      change_move_time = millis() + attack[move_step][2];
    }else{
      estado = Controller;
    }
  }else{
    if(millis() < change_move_time){
      set_motor_power(RIGHT, attack[move_step][1]);
      set_motor_power(LEFT,  attack[move_step][0]);
      motor_control(RIGHT, get_motor_power(LEFT));
      motor_control(LEFT,  get_motor_power(RIGHT));
    }else{
      should_change = 1;
    }
  }
}

int get_state(){
    return estado;
}

void set_state(State novo_estado){
    estado = novo_estado;
}