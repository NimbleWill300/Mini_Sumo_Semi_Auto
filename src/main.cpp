#include <Arduino.h>
#include <PS4Controller.h>
#include "motors.hpp"

#define EN_B GPIO_NUM_15
#define BI1 GPIO_NUM_2
#define BI2 GPIO_NUM_4
#define STANDBY GPIO_NUM_16
#define AI2 GPIO_NUM_17
#define AI1 GPIO_NUM_5
#define EN_A GPIO_NUM_18

enum State{
  Controller,
  Square,
  Circle
};

enum ControllerState{
  NormalMode,
  ProMode
};
ControllerState controllerstate = NormalMode;

typedef struct{
  int r = 255;
  int g = 255;
  int b = 255;
}LedColors;
LedColors colors;
int first_change = 1;

typedef struct{
  int powerRight;
  int powerLeft;
}Motors;
Motors motor_power;

State estado = Controller;
State ultimo_estado = estado;
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

int calculate_deadZone(int input){
  if(input > -MOTOR_DEADZONE && input < MOTOR_DEADZONE) return 0;
  else return input;
}

int update_controller(ControllerState state){
  if(state == NormalMode){
    int x = 0;
    int r2 = 0, l2 = 0;
    if (PS4.RStickX()) {
      x = PS4.RStickX();
    }
    if(PS4.R2Value()){
      r2 = PS4.R2Value();
    }else{
      r2 = 0;
    }
    if(PS4.L2Value()){
      l2 = PS4.L2Value();
    }else{
      l2 = 0;
    }
    // r2 = calculate_deadZone(r2);     //Como o valor nao varia q nem o joystick 
    // l2 = calculate_deadZone(l2);     //nao vi necessidade de colocar deadZone
    x = calculate_deadZone(x);
    int speed = r2 - l2;
    int mappedX = map(x, -127, 127, -MAX_MOTOR, MAX_MOTOR);
    
    motor_power.powerRight = speed - mappedX;
    motor_power.powerLeft  = speed + mappedX;
    
  }else{
    int right = 0;
    int left = 0;
    if (PS4.RStickY()) {
      right = PS4.RStickY();
    }
    if (PS4.LStickY()) {
      left = PS4.LStickY();
    }
    
    right = calculate_deadZone(right);
    left = calculate_deadZone(left);

    int mappedRight = map(right, -127, 127, -MAX_MOTOR, MAX_MOTOR);
    int mappedLeft  = map(left , -127, 127, -MAX_MOTOR, MAX_MOTOR);
    
    motor_power.powerRight = mappedRight;
    motor_power.powerLeft  = mappedLeft;
    
  }

  if(motor_power.powerRight == 0 && motor_power.powerLeft == 0){
    return 0;
  }else{
    return 1;
  }
}

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

void change_led_color(int r, int g, int b){
    if(r != colors.r || g != colors.g || b != colors.b || first_change){
      first_change = 0;
      colors.r = r;
      colors.g = g;
      colors.b = b;
      PS4.setLed(r, g, b);
      PS4.sendToController();
    }
}

void run_state(){
  if(estado == Controller){
    if(controllerstate == NormalMode){
      change_led_color(0, 255, 0);      //Verde
    }else{
      change_led_color(255, 0, 0);    //Vermelho
    }
    motor_control(LEFT, motor_power.powerRight);
    motor_control(RIGHT, motor_power.powerLeft);
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
      motor_power.powerRight = attack[move_step][1];
      motor_power.powerLeft  = attack[move_step][0];
      motor_control(RIGHT,  motor_power.powerLeft);
      motor_control(LEFT, motor_power.powerRight);
    }else{
      should_change = 1;
    }
  }

}

void setup() {
  pinMode(EN_B, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(AI1, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(STANDBY, OUTPUT);
  digitalWrite(STANDBY, HIGH);
  Serial.begin(115200);
  PS4.begin("2c:dc:d7:39:25:74");
  Serial.println("Ready.");
  estado = Controller;
}

void loop() {
  if (PS4.isConnected()) {
    update_state();
    run_state();
    
    Serial.print(motor_power.powerLeft);
    Serial.print("\t");
    Serial.println(motor_power.powerRight);

  }else{
    motor_control(RIGHT, 0);
    motor_control(LEFT, 0);
  }
}