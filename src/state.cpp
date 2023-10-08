#include "state.hpp"

State estado = Controller;
State ultimo_estado = estado;
ControllerState controllerstate = NormalMode;
int should_change = 0;
int move_step = 0;
int change_move_time;

int const num_square_left_square_steps = 4;
int square_left_attack[num_square_left_square_steps][3]{
      {-255, 255, 60},    //vira esquerda
      {255, 255, 150},     //reto
      {255, -255, 120},     //vira direita
      {255, 255, 150}      //ret0
};

int const num_square_right_steps = 4;
int square_right_attack[num_square_right_steps][3]{
      {255, -255, 60},    //vira direita
      {255, 255, 150},     //reto
      {-255, 255, 120},     //vira esquerda
      {255, 255, 150}      //ret0
};

int const num_right_dodge_steps = 2;
int right_dodge[num_right_dodge_steps][3]{
      {255, -255, 60},      //vira direita
      {-255, -255, 150}     //ré
};

int const num_left_dodge_steps = 2;
int left_dodge[num_left_dodge_steps][3]{
      {-255,  255, 60},     //vira esquerda
      {-255, -255, 150}     //ré
};

void update_state(){
  if(update_controller(controllerstate)){         //verifica se o controle foi movido, pois se sim
    estado = Controller;                          //cancela o movimento e volta ao modo controlado
    return;
  }
  if(PS4.Square() && PS4.Left() && estado != SquareLeft && estado != LeftDodge){           //ataque lateral pela esquerda
    estado = SquareLeft;
    should_change = 0;
    change_move_time = millis() + square_left_attack[0][2];
    move_step = 0;
  }
  if(PS4.Square() && PS4.Right() && estado != SquareRight && estado != RightDodge){           //ataque lateral pela direita
    estado = SquareRight;
    should_change = 0;
    change_move_time = millis() + square_right_attack[0][2];
    move_step = 0;
  }
  if(PS4.Right() && estado != SquareRight && estado != RightDodge){   //Evasiva para direita
    estado = RightDodge;
    should_change = 0;
    change_move_time = millis() + right_dodge[0][2];
    move_step = 0;
  }
  if(PS4.Left() && estado != SquareLeft && estado != LeftDodge){   //Evasiva para esquerda
    estado = LeftDodge;
    should_change = 0;
    change_move_time = millis() + left_dodge[0][2];
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
      // Serial.println("NormalMode");
    }else{
      change_led_color(255, 0, 0);    //Vermelho
      // Serial.println("ProMode");
    }
    motor_control(LEFT, get_motor_power(RIGHT));
    motor_control(RIGHT, get_motor_power(LEFT));
    return;
  }

  int num_steps;
  int (*attack)[3];
  switch(estado){
    case SquareLeft:
      change_led_color(220, 20, 60);
      num_steps = num_square_left_square_steps;
      attack = square_left_attack;
      // Serial.println("Square Left");
      break;
    case SquareRight:
      change_led_color(60, 220, 20);
      num_steps = num_square_right_steps;
      attack = square_right_attack;
      // Serial.println("Square Right");
      break;
    case RightDodge:
      change_led_color(0, 191, 255);
      num_steps = num_right_dodge_steps;
      attack = right_dodge;
      // Serial.println("Right dodge");
      break;
    case LeftDodge:
      change_led_color(102, 205, 170);
      num_steps = num_left_dodge_steps;
      attack = left_dodge;
      // Serial.println("Left dodge");
      break;
  }
  
  if(should_change){
    should_change = 0;
    if(move_step + 1< num_steps){
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